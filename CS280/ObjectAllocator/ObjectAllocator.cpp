#include "ObjectAllocator.h"
#include <cstring>

ObjectAllocator::ObjectAllocator(size_t ObjectSize, const OAConfig &config)
{
	try
	{
		PageList_ = nullptr;
		FreeList_ = nullptr;

		stats_.ObjectSize_ = ObjectSize;
		config_ = config;

		config_.LeftAlignSize_ = 0;
		config_.InterAlignSize_ = 0;

		size_t pageSize = sizeof(GenericObject *) + config_.ObjectsPerPage_ * (ObjectSize + (config_.PadBytes_ * 2) + config_.HBlockInfo_.size_);
		stats_.PageSize_ = pageSize;

		allocate_new_page();
	}
	catch (std::bad_alloc &e)
	{
		throw OAException(OAException::OA_EXCEPTION::E_NO_MEMORY, e.what());
	}
}

// Destroys the ObjectManager (never throws)
ObjectAllocator::~ObjectAllocator()
{
	GenericObject *currentPage = PageList_;
	while (currentPage != nullptr)
	{
		GenericObject *nextPage = currentPage->Next;
		delete[] reinterpret_cast<char *>(currentPage);
		currentPage = nextPage;
	}

	stats_ = OAStats();
	config_ = OAConfig();
}
// Take an object from the free list and give it to the client (simulates new)
// Throws an exception if the object can't be allocated. (Memory allocation problem)
void *ObjectAllocator::Allocate(const char *label)
{
	// check if the freelist is empty
	if (FreeList_ == nullptr)
	{
		// allocate new page
		try
		{
			allocate_new_page();
		}
		catch (OAException &e)
		{
			throw e;
		}
	}

	// get the object from the freelist
	GenericObject *object = FreeList_;
	MemBlockInfo *blockInfo = reinterpret_cast<MemBlockInfo *>(object);
	FreeList_ = object->Next;

	// update the stats
	stats_.Allocations_++;
	stats_.ObjectsInUse_++;
	stats_.FreeObjects_--;
	stats_.MostObjects_ = std::max(stats_.MostObjects_, stats_.ObjectsInUse_);

	// update the header
	blockInfo->alloc_num = stats_.Allocations_;
	blockInfo->in_use = 1;
	blockInfo->label = const_cast<char *>(label);

	// add memory signature
	unsigned char *objectAsChar = reinterpret_cast<unsigned char *>(object);

	memset(objectAsChar, ALLOCATED_PATTERN, stats_.ObjectSize_);

	if (config_.HBlockInfo_.type_ == OAConfig::HBLOCK_TYPE::hbBasic)
	{
		memset(objectAsChar - config_.PadBytes_ - 1, 1, 1);
		memcpy(objectAsChar - config_.PadBytes_ - config_.HBlockInfo_.size_, reinterpret_cast<unsigned char *>(&stats_.Allocations_), sizeof(unsigned));
	}
	else if (config_.HBlockInfo_.type_ == OAConfig::HBLOCK_TYPE::hbExtended)
	{
		memset(objectAsChar - config_.PadBytes_ - 1, 1, 1);
		memcpy(objectAsChar - config_.PadBytes_ - config_.BASIC_HEADER_SIZE, reinterpret_cast<unsigned char *>(&stats_.Allocations_), sizeof(unsigned));

		unsigned short counter = *(objectAsChar - config_.PadBytes_ - config_.HBlockInfo_.size_ + 1);
		++counter;
		memcpy(objectAsChar - config_.PadBytes_ - config_.HBlockInfo_.size_ + 1, reinterpret_cast<unsigned char *>(&counter), sizeof(unsigned short));
	}

	// return the object
	return reinterpret_cast<void *>(objectAsChar + config_.HBlockInfo_.size_);
}

// Returns an object to the free list for the client (simulates delete)
// Throws an exception if the the object can't be freed. (Invalid object)
void ObjectAllocator::Free(void *Object)
{
	if (Object == nullptr)
	{
		throw OAException(OAException::OA_EXCEPTION::E_CORRUPTED_BLOCK, "Object is null");
	}

	MemBlockInfo *blockInfo = reinterpret_cast<MemBlockInfo *>(reinterpret_cast<unsigned char *>(Object) - config_.HBlockInfo_.size_);

	if (blockInfo->in_use == 0)
	{
		throw OAException(OAException::OA_EXCEPTION::E_MULTIPLE_FREE, "Object has already been freed");
	}

	blockInfo->in_use = 0;
	blockInfo->label = nullptr;
	char *object = reinterpret_cast<char *>(Object) - (config_.HBlockInfo_.size_);
	memset(object, FREED_PATTERN, stats_.ObjectSize_);

	// add the object to the freelist
	GenericObject *genericObject = reinterpret_cast<GenericObject *>(object);
	genericObject->Next = FreeList_;
	FreeList_ = genericObject;

	// update the stats
	stats_.ObjectsInUse_--;
	stats_.FreeObjects_++;
	stats_.Deallocations_++;

	unsigned char *objectAsChar = reinterpret_cast<unsigned char *>(genericObject);
	if (config_.HBlockInfo_.type_ == OAConfig::HBLOCK_TYPE::hbBasic)
	{
		memset(objectAsChar - config_.PadBytes_ - config_.HBlockInfo_.size_, 0, config_.HBlockInfo_.size_);
	}
	else if (config_.HBlockInfo_.type_ == OAConfig::HBLOCK_TYPE::hbExtended)
	{
		unsigned short counter = *(objectAsChar - config_.PadBytes_ - config_.HBlockInfo_.size_ + 1);
		memset(objectAsChar - config_.PadBytes_ - config_.HBlockInfo_.size_, 0, config_.HBlockInfo_.size_);
		memcpy(objectAsChar - config_.PadBytes_ - config_.HBlockInfo_.size_ + 1, reinterpret_cast<unsigned char *>(&counter), sizeof(unsigned short));
	}
}

// Calls the callback fn for each block still in use
// NOTE:  DumpMemoryInUse method returns the number of blocks in use by the client
unsigned ObjectAllocator::DumpMemoryInUse(DUMPCALLBACK fn) const
{
	unsigned int count = 0;
	GenericObject *currentPage = PageList_;
	MemBlockInfo *blockInfo = reinterpret_cast<MemBlockInfo *>(reinterpret_cast<unsigned char *>(currentPage) - config_.HBlockInfo_.size_);
	while (currentPage != nullptr)
	{
		GenericObject *currentObject = currentPage;
		for (unsigned int i = 0; i < config_.ObjectsPerPage_; i++)
		{
			// check if the object is in use
			if (blockInfo->in_use == 1)
			{
				fn(reinterpret_cast<void *>(reinterpret_cast<unsigned char *>(currentObject) + config_.HBlockInfo_.size_), stats_.ObjectSize_);
				count++;
			}
			currentObject = reinterpret_cast<GenericObject *>(reinterpret_cast<unsigned char *>(currentObject) + config_.HBlockInfo_.size_ + stats_.ObjectSize_);
		}
		currentPage = currentPage->Next;
	}
	return count;
}

// Calls the callback fn for each block that is potentially corrupted
unsigned ObjectAllocator::ValidatePages(VALIDATECALLBACK fn) const
{
	unsigned int count = 0;
	GenericObject *currentPage = PageList_;
	MemBlockInfo *blockInfo = reinterpret_cast<MemBlockInfo *>(reinterpret_cast<unsigned char *>(currentPage) - config_.HBlockInfo_.size_);
	while (currentPage != nullptr)
	{
		GenericObject *currentObject = currentPage;
		for (unsigned int i = 0; i < config_.ObjectsPerPage_; i++)
		{
			// check if the object is corrupted
			if (blockInfo->in_use == 1 && blockInfo->alloc_num == 0)
			{
				fn(reinterpret_cast<void *>(reinterpret_cast<unsigned char *>(currentObject) + config_.HBlockInfo_.size_), stats_.ObjectSize_);
				count++;
			}
			currentObject = reinterpret_cast<GenericObject *>(reinterpret_cast<unsigned char *>(currentObject) + config_.HBlockInfo_.size_ + stats_.ObjectSize_);
		}
		currentPage = currentPage->Next;
	}
	return count;
}

// Frees all empty pages (extra credit)
unsigned ObjectAllocator::FreeEmptyPages()
{
	return 0;
}

// Returns true if FreeEmptyPages and alignments are implemented
bool ObjectAllocator::ImplementedExtraCredit(void)
{
	return false;
}

// Testing/Debugging/Statistic methods
void ObjectAllocator::SetDebugState(bool State)
{
	config_.DebugOn_ = State;
} // true=enable, false=disable

const void *ObjectAllocator ::GetFreeList(void) const
{
	return FreeList_;
} // returns a pointer to the internal free list

const void *ObjectAllocator::GetPageList(void) const
{
	return PageList_;
} // returns a pointer to the internal page list

OAConfig ObjectAllocator::GetConfig(void) const
{
	return config_;
} // returns the configuration parameters

OAStats ObjectAllocator::GetStats(void) const
{
	return stats_;
} // returns the statistics for the allocator

void ObjectAllocator::allocate_new_page()
{
	try
	{
		// check if the maximum number of pages is reached
		if (stats_.PagesInUse_ == config_.MaxPages_ && config_.MaxPages_ != 0)
		{
			throw OAException(OAException::OA_EXCEPTION::E_NO_PAGES, "Maximum number of pages reached");
		}

		// allocate memory for the new page
		GenericObject *newPage = new GenericObject[stats_.PageSize_];

		if (newPage == nullptr)
		{
			throw OAException(OAException::OA_EXCEPTION::E_NO_MEMORY, "Failed to allocate memory for new page");
		}

		// add the new page to the page list
		newPage->Next = PageList_;
		PageList_ = newPage;

		// update the stats
		stats_.PagesInUse_++;
		stats_.FreeObjects_ += config_.ObjectsPerPage_;
		stats_.MostObjects_ = std::max(stats_.MostObjects_, stats_.ObjectsInUse_);


		// create the freelist for the new page
		for (unsigned int i = 0; i < config_.ObjectsPerPage_; i++)
		{
			char *objectStart = reinterpret_cast<char *>(newPage) + sizeof(GenericObject *) + config_.HBlockInfo_.size_ + i * (stats_.ObjectSize_ + config_.PadBytes_ * 2 + config_.HBlockInfo_.size_);
			memset(objectStart, PAD_PATTERN, config_.PadBytes_);
			memset(objectStart + config_.PadBytes_, UNALLOCATED_PATTERN, stats_.ObjectSize_);
			memset(objectStart + stats_.ObjectSize_ + config_.PadBytes_, PAD_PATTERN, config_.PadBytes_);
			GenericObject *object = reinterpret_cast<GenericObject *>(objectStart + config_.PadBytes_);
			reinterpret_cast<MemBlockInfo *>(object)->in_use = 0;
			object->Next = FreeList_;
			FreeList_ = object;
		}
	}
	catch (std::bad_alloc &e)
	{
		throw OAException(OAException::OA_EXCEPTION::E_NO_MEMORY, e.what());
	}
}