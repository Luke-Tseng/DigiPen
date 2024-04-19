#ifndef WRRMMAP_H
#define WRRMMAP_H
#include <atomic>
#include <map>
#include "HPRecType.h"

template <class K, class V>
class WRRMMap
{
private:
    std::atomic<void *> pMap_;

public:
    void Update(const K &key, const V &value)
    {
        void *pOld = pMap_.load();
        std::map<K, V> *pNew = 0;

        do
        {
            if (pNew)
                delete pNew;
            if (pOld)
            {
                pNew = new std::map<K, V>(*static_cast<std::map<K, V> *>(pOld));
            }
            else
            {
                pNew = new std::map<K, V>();
            }
            (*pNew)[key] = value;
        } while (!pMap_.compare_exchange_strong(pOld, pNew));
        HPRecType::Retire(pOld);
    }

    V Lookup(const K &key)
    {
        HPRecType *pRec = HPRecType::Acquire();

        std::map<K, V> *ptr;
        do
        {
            ptr = static_cast<std::map<K, V> *>(pMap_.load());
            pRec->pHazard_ = ptr;
        } while (ptr != pMap_.load());

        V result = (*ptr)[key];

        HPRecType::Release(pRec);
        return result;
    }
};

#endif