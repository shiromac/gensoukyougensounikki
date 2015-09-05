#pragma once

class VariationNumber;
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <map>

typedef boost::shared_ptr<VariationNumber> pVariationNumber;
typedef boost::weak_ptr<VariationNumber> wpVariationNumber;

namespace VariationValue_Enum{
	enum OutRangeMode
	{
		Mode_Cramp = 0,//引き伸ばし
		Mode_Wrap,//繰り返し
	};
};

//doubleからTを返す関数クラス
template <class T> class VariationValue
{

protected:
	
	template <class T> class VariationValue_Cascade;
	template <class T> class VariationValue_Cascade_Const;

	VariationValue_Enum::OutRangeMode mode_;
	double sumLength_;
	std::map<double, boost::shared_ptr<VariationValue_Cascade<T>>> lengthSep_;
	T defaultOutput;
public:
	VariationValue():mode_(VariationValue_Enum::Mode_Cramp),sumLength_(0)
	{
		return;
	};
	virtual ~VariationValue(){};

public:
	bool empty() const
	{
		return lengthSep_.empty();
	};
	void clear()
	{
		lengthSep_.clear();
	};
	virtual T operator()(double argument)
	{
		if(lengthSep_.empty()) return defaultOutput;//空
		std::map<double, boost::shared_ptr<VariationValue_Cascade<T>>>::iterator itr,tempitr;
	 
		double chip_argument = -1.0;
		if(mode_ == VariationValue_Enum::Mode_Cramp)
		{//引き伸ばし
			if(sumLength_ <= argument)
			{
				//大きい
				itr = --lengthSep_.end();
				chip_argument = itr->second->Length();
			}
			else if(argument < 0)
			{
				//小さい
				itr = lengthSep_.begin();
				chip_argument = 0;
			}
			else
			{
				itr = lengthSep_.upper_bound(argument);
				if(itr == lengthSep_.begin())
				{
					chip_argument = argument;
				}
				else
				{
					tempitr = itr;
					chip_argument = argument - (--tempitr)->first;
				}
			}
		}
		else if(mode_ == VariationValue_Enum::Mode_Wrap)
		{
			int roopcount = (int)floor(argument / sumLength_);
			argument -= roopcount*sumLength_;

			itr = lengthSep_.upper_bound(argument);
			if(itr == lengthSep_.begin())
			{
				chip_argument = argument;
			}
			else
			{
				tempitr = itr;
				chip_argument = argument - (--tempitr)->first;
			}
		}

		return (*(itr->second))(chip_argument);
	};
	virtual bool operator==(const VariationValue& rother)const
	{
		bool flag = ((*this).defaultOutput == rother.defaultOutput);
		flag &= ((*this).lengthSep_ == rother.lengthSep_);
		return flag;
	}
	virtual T value(double argument){return (*this)(argument);};
	virtual double sumLength() const {return sumLength_;};
	void setOutRangeMode(VariationValue_Enum::OutRangeMode mode){mode_ = mode;};
	virtual void addCascade(boost::shared_ptr<VariationValue_Cascade<T>> pchip)
	{
		//Sum増加
		sumLength_ += pchip->Length();
		lengthSep_.insert(map<double, boost::shared_ptr<VariationValue_Cascade<T>>>::value_type(sumLength_,pchip));
	};


	//VariationValue構成クラス
	template <class T> class VariationValue_Cascade
	{
	public:
		VariationValue_Cascade(){};
		virtual ~VariationValue_Cascade(){};

	public:
		virtual T operator()(double argument) = 0;
		virtual double Length() = 0;
	};

	//Const
	template <class T> class VariationValue_Cascade_Const 
		: public VariationValue_Cascade<T>
	{
	public:
		VariationValue_Cascade_Const(const T& value, double length):
		  value_(value), length_(length){};
		  virtual ~VariationValue_Cascade_Const(){};

	public:
		virtual T operator()(double argument)
		{
			return value_;
		};
		virtual double Length(){return length_;};
	private:
		T value_;
		double length_;
	};

	//Linear
	template <class T> class VariationValue_Cascade_Linear
		: public VariationValue_Cascade<T>
	{
	public:
		VariationValue_Cascade_Linear(const T& first_value, const T& last_value, double length):
		  first_value_(first_value), last_value_(last_value), length_(length){};
		  virtual ~VariationValue_Cascade_Linear(){};

	public:
		virtual T operator()(double argument)
		{
			double t = argument/length_;
			return (1 - t)*first_value_ + t*last_value_;
		};
		virtual double Length(){return length_;};
	private:
		T first_value_;
		T last_value_;
		double length_;
	};

	//2B-spline
	template <class T> class VariationValue_Cascade_2Bspline
		: public VariationValue_Cascade<T>
	{
	public:
		VariationValue_Cascade_2Bspline(const T& first_value,
									const T& second_value,
									const T& last_value,
									double length):
			  first_value_(first_value),
			  second_value_(second_value),
			  last_value_(last_value),
			  length_(length){};
		  virtual ~VariationValue_Cascade_2Bspline(){};

	public:
		virtual T operator()(double argument)
		{
			double t = argument/length_;
			return (1-t)*(1-t)*first_value_ + 2*(1-t)*t*second_value_ + t*t*last_value_;
		};
		virtual double Length(){return length_;};
	private:
		T first_value_;
		T second_value_;
		T last_value_;
		double length_;
	};
	//Bezier
	template <class T> class VariationValue_Cascade_Bezier
		: public VariationValue_Cascade<T>
	{
	public:
		VariationValue_Cascade_Bezier(const T& first_value,
									const T& second_value,
									const T& third_value,
									const T& last_value,
									double length):
			  first_value_(first_value),
			  second_value_(second_value),
			  third_value_(third_value),
			  last_value_(last_value),
			  length_(length){};
		  virtual ~VariationValue_Cascade_Bezier(){};

	public:
		virtual T operator()(double argument)
		{
			double t = argument/length_;
			return (1-t)*(1-t)*(1-t)*first_value_ + 3*(1-t)*(1-t)*t*second_value_ + 3*(1-t)*t*t*third_value_ + t*t*t*last_value_;
		};
		virtual double Length(){return length_;};
	private:
		T first_value_;
		T second_value_;
		T third_value_;
		T last_value_;
		double length_;
	};
public:
	//暗黙方変換特殊コンストラクタ
	VariationValue(const T& value):mode_(VariationValue_Enum::Mode_Cramp),sumLength_(0)
	{
		setDefault(value);
	};

	void setDefault(const T& value)
	{
		defaultOutput = value;
	};
public:
	//省略形
	void addCascade(const T& value, double length)//定数
	{
		addCascade(boost::shared_ptr<VariationValue_Cascade_Const<T>>(new VariationValue_Cascade_Const<T>(value, length)) );
	};
	void addCascade(const T& first_value, const T& last_value, double length)//線形
	{
		addCascade(boost::shared_ptr<VariationValue_Cascade_Linear<T>>(new VariationValue_Cascade_Linear<T>(first_value, last_value, length)) );
	};
	void addCascade(const T& first_value, const T& second_value, const T& last_value, double length)//2Bスプライン
	{
		addCascade(boost::shared_ptr<VariationValue_Cascade_2Bspline<T>>(new VariationValue_Cascade_2Bspline<T>(first_value, second_value, last_value, length)) );
	};
	void addCascade(const T& first_value, const T& second_value, const T& third_value, const T& last_value, double length)//ベジェ
	{
		addCascade(boost::shared_ptr<VariationValue_Cascade_Bezier<T>>(new VariationValue_Cascade_Bezier<T>(first_value, second_value, third_value, last_value, length)) );
	};

};
