/*
 * Average.h
 *
 *  Created on: Nov 18, 2015
 *      Author: yankai
 */

#ifndef OpenKAI_src_Filter_Average_H_
#define OpenKAI_src_Filter_Average_H_

#include "FilterBase.h"

namespace kai
{

	template <class T>
	class Average : public FilterBase<T>
	{
	public:
		Average<T>()
		{
			FilterBase<T>::m_nW = 2;
		}
		virtual ~Average()
		{
		}

		bool init(int nW)
		{
			FilterBase<T>::m_nW = nW;
			if (FilterBase<T>::m_nW < 2)
				FilterBase<T>::m_nW = 2;

			FilterBase<T>::reset();

			return true;
		}

		T input(T v)
		{
			if(!FilterBase<T>::add(v))
			{
				FilterBase<T>::m_v = v;
				return FilterBase<T>::m_v;
			}

			T tot = 0.0;
			for (int i = 0; i < FilterBase<T>::m_nW; i++)
				tot += FilterBase<T>::m_qV.at(i);

			FilterBase<T>::m_v = tot / (T)FilterBase<T>::m_nW;
			return FilterBase<T>::m_v;
		}
	};

}
#endif
