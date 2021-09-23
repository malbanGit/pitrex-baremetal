
#ifndef _POOL_H
#define _POOL_H

#include "GSList.h"
#include <stdint.h>

template <class T, int itemsPerBlock = 50>
class PoolAllocator
{
public:
	T *Allocate()
	{
		// Find a block with free space
		while (true)
		{
			for (auto i = _pool.begin(); i != _pool.end(); ++i)
			{
				T *p = (*i)->Allocate();
				if (p)
					return p;
			}
			_pool.push_front(new PoolBlock());
		}
		return NULL;
	}

	void Free(T *doomed)
	{
		// Find the pool block that this item is in
		for (auto i = _pool.begin(); i != _pool.end(); ++i)
		{
			if ((*i)->Contains(doomed))
			{
				(*i)->Free(doomed);
                if ((*i)->IsAllFree())
                {
                    delete *i;
                    _pool.erase(i);
                }
				break;
			}
		}
	}

private:
	class PoolBlock
	{
	public:
		PoolBlock()
		{
			// Put every element in the block into a free list
			*(T **) &block[0] = NULL;
			for (size_t i = 1; i < itemsPerBlock; ++i)
			{
				*(T **) ((T *) block + i) = (T *) block + i - 1;
			}
			freeHead = (T *) block + itemsPerBlock - 1;
		}
        
        PoolBlock(const PoolBlock &); // unimplemented

		bool Contains(const T *p)
		{
			return p >= (const T *) block && p < (const T *) block + itemsPerBlock;
		}

		void Free(T *p)
		{
			*(T **) p = freeHead;
			freeHead = p;
		}

		T *Allocate()
		{
			if (freeHead)
			{
				T *p = freeHead;
				freeHead = *(T **) freeHead;
				return p;
			}
			return NULL;
		}
        
        bool IsAllFree() const
        {
            int freeCount = 0;
            T *p = freeHead;
            while (p)
            {
                ++freeCount;
                p = *(T **) p;
            }
            return freeCount == itemsPerBlock;
        }

	private:
		T *freeHead;
		uint32_t block[itemsPerBlock * sizeof(T) / sizeof(uint32_t)];
	};

	list<PoolBlock *> _pool;
};

#endif
