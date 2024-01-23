#include "print_queue.h"

////////////////////////////////////////////////////////////
/* PrintQueue implementation */
////////////////////////////////////////////////////////////

/* constructor
 * pointer to a simulator that's running it
 */
PrintQueue::PrintQueue(Simulator *_psim) : psim(_psim), num_available_printers(0), printer_status(), jobs_in_queue()
{
}

PrintQueue::PrintQueue(const PrintQueue &rhs) : psim(rhs.psim), num_available_printers(rhs.num_available_printers), printer_status(rhs.printer_status), jobs_in_queue(rhs.jobs_in_queue)
{
}

PrintQueue &PrintQueue::operator=(const PrintQueue &rhs)
{
	psim = rhs.psim;
	num_available_printers = rhs.num_available_printers;
	printer_status = rhs.printer_status;
	jobs_in_queue = rhs.jobs_in_queue;
	return *this;
}

/* to use a printer we need to register it with the print queue first */
void PrintQueue::RegisterPrinter(int ID, double speed)
{
	/* pair of (ID and a pair (available, speed) ) */
	//	printer_status.insert(
	//			std::make_pair<int,std::pair<bool,double> >
	//			         (ID,std::make_pair<bool,double>(true,speed) )
	//			);
	// next line is equivalent to the above insert
	printer_status[ID] = std::make_pair(true, speed);
	// printer is available by default
	++num_available_printers;
}

/* implements 2) from PrintQueue */
void PrintQueue::JobFinished(double current_time, int printerID, PrintJob job)
{
	ID2pair::iterator it = printer_status.find(printerID);

	if (it == printer_status.end())
		throw "invalid printerID";

	std::cout << "Printer " << printerID << " finished job "
			  << job.jobID << " at time " << current_time << std::endl;

	if (!jobs_in_queue.empty())
	{ // immediately assign another job
		// finds new job and removes from queue
		PrintJob new_job = jobs_in_queue.top().first;
		jobs_in_queue.pop();
		// calculate finish time
		double finish_time = current_time + new_job.size / it->second.second;
		// create new finish event with calculated time
		psim->AddEvent(new EventJobFinished(finish_time, this, printerID, new_job));

		std::cout << "Printer " << printerID << " assigned new job "
				  << new_job.jobID << " at time " << current_time << std::endl;
	}
	else
	{ // no jobs in the queue - printer rests
		++num_available_printers;
		it->second.first = true;
	}
}

/* implements 3) from PrintQueue */
void PrintQueue::NewJobArrived(double current_time, PrintJob job)
{
	std::cout << "New job " << job.jobID
			  << " at time " << current_time << std::endl;

	if (num_available_printers > 0)
	{ // self-explanatory
		// find an available printer
		ID2pair::iterator it = printer_status.begin();
		while (it != printer_status.end() && it->second.first == false)
		{
			++it;
		}

		// if condition holds, we have a problem with data:
		// num_available_printers>0 but no printer seem to be available
		if (it == printer_status.end())
			throw "corrupted data";

		// calculate finsih time
		double finish_time = current_time + job.size / it->second.second;
		// creates new finish event
		psim->AddEvent(new EventJobFinished(finish_time, this, it->first, job));
		// set printer to unavaliable
		it->second.first = 0;
		num_available_printers--;

		std::cout << "Job assigned to printer " << it->first
				  << " at time " << current_time << std::endl;
	}
	else
	{
		std::cout << "No available printers - put in print queue\n";
		jobs_in_queue.push(std::make_pair(job, current_time));
	}
}

////////////////////////////////////////////////////////////
/* EventJobFinished implementation */
////////////////////////////////////////////////////////////

EventJobFinished::EventJobFinished(double _when, PrintQueue *_pqueue, int _printerID, PrintJob _job)
	: TimedCommand(), pqueue(_pqueue), printerID(_printerID), job(_job)
{
	SetWhen(_when);
}

EventJobFinished::EventJobFinished(const EventJobFinished &rhs)
	: TimedCommand(rhs), pqueue(rhs.pqueue), printerID(rhs.printerID), job(rhs.job)
{
}

EventJobFinished &EventJobFinished::operator=(const EventJobFinished &rhs)
{
	if (this != &rhs)
	{
		TimedCommand::operator=(rhs);
		pqueue = rhs.pqueue;
		printerID = rhs.printerID;
		job = rhs.job;
	}
	return *this;
}

void EventJobFinished::Execute()
{
	// calls jobfinished
	pqueue->JobFinished(When(), printerID, job);
}

////////////////////////////////////////////////////////////
/* EventNewJobArrived implementation */
////////////////////////////////////////////////////////////
EventNewJobArrived::EventNewJobArrived(
	double when,
	const PrintJob &_job,
	PrintQueue *_pqueue,
	void (PrintQueue::*_p_method)(double, PrintJob))
	: TimedCommand(), job(_job), pqueue(_pqueue), p_method(_p_method)
{
	SetWhen(when);
}

EventNewJobArrived::EventNewJobArrived(const EventNewJobArrived &rhs)
	: TimedCommand(rhs), job(rhs.job), pqueue(rhs.pqueue), p_method(rhs.p_method)
{
}

EventNewJobArrived &EventNewJobArrived::operator=(const EventNewJobArrived &rhs)
{
	if (this != &rhs)
	{
		TimedCommand::operator=(rhs);
		job = rhs.job;
		pqueue = rhs.pqueue;
		p_method = rhs.p_method;
	}
	return *this;
}

void EventNewJobArrived::Execute()
{
	(pqueue->*p_method)(When(), job);
}
