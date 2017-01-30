#pragma once

#include <boost/weak_ptr.hpp>

class GensouGageViewDelegate
{
public:
	virtual double short_live_power() = 0;
	virtual double long_live_power() = 0;
};

typedef boost::weak_ptr<GensouGageViewDelegate> wpGensouGageViewDelegate;

class GensouGage : public GensouGageViewDelegate
{
public:
	GensouGage(void);
	virtual ~GensouGage(void);

	virtual double short_live_power();
	virtual double long_live_power();
	double sum_power();
	
	void add_short_live_power(double power);
	void keep_short_live_power();
	void add_long_live_power(double power);

	void reset_power_to_zero();
	
	void pass_turn(bool no_refresh_short_live_power, bool no_loss_long_live_power);

private:
	double short_live_power_;
	double long_live_power_;

	bool short_live_power_keep_;
	
	void refresh_short_live_power(double trans_rate);
	void loss_long_live_power(double loss_rate, double min_loss);

	double loss_rate();
	double formalize(double value);
};

class GensouGageViewImpl;
typedef boost::shared_ptr<GensouGageViewImpl> pGensouGageViewImpl;

struct IDirect3DDevice9;

class GensouGageView
{
public:
	GensouGageView(void);
	virtual ~GensouGageView(void);

	void init();
	void process();
	void draw(IDirect3DDevice9 *pDev);

	void refreshView();

	wpGensouGageViewDelegate delegate;

private:
	double short_live_power_;
	double long_live_power_;

	pGensouGageViewImpl member;
};
