#include "GensouGage.h"
#include <math.h>
#include <algorithm>
#include <boost/make_shared.hpp>
#include "cNumField.h"
#include "VariationNumber.h"
#include "cDungeonSystem.h"

GensouGage::GensouGage(void):short_live_power_(0), long_live_power_(0)
{
}

GensouGage::~GensouGage(void)
{
}


double GensouGage::short_live_power() {
	return short_live_power_;
}
double GensouGage::long_live_power() {
	return long_live_power_;
}
double GensouGage::sum_power() {
	return short_live_power() + long_live_power();
}
	
void GensouGage::add_short_live_power(double power) {
	short_live_power_ += power;
	short_live_power_ = formalize(short_live_power_);
}
void GensouGage::add_long_live_power(double power) {
	long_live_power_ += power;
	long_live_power_ = formalize(long_live_power_);
}

void GensouGage::reset_power_to_zero() {
	short_live_power_ = 0;
	long_live_power_ = 0;
}
	
void GensouGage::refresh_short_live_power(double trans_rate)
{
	long_live_power_ += short_live_power_ * trans_rate;
	short_live_power_ = 0;
}
void GensouGage::loss_long_live_power(double loss_rate, double min_loss)
{
	double loss = long_live_power_*(1-loss_rate);
	loss = max(min_loss, loss);
	long_live_power_ -= loss;
	long_live_power_ = formalize(long_live_power_);
}

void GensouGage::pass_turn(bool no_refresh_short_live_power, bool no_loss_long_live_power)
{
	if (no_loss_long_live_power) {
		loss_long_live_power(loss_rate(), 0.1);
	}
	if (no_refresh_short_live_power) {
		refresh_short_live_power(0.1);
	}
}

double GensouGage::loss_rate() {
	return 0.99;
}

double GensouGage::formalize(double value) {
	return floor(value*10.0) / 10.0;
}

//------------------------------------------------
class GensouGageViewImpl
{
public:
	cNumField long_live_power;
	VariationValue<double> long_size;
	VariationValue<cColor> long_color;
	VariationValue<double> long_distance;
	double long_count;

	cNumField short_live_power;
	VariationValue<double> short_size;
	VariationValue<cColor> short_color;
	VariationValue<double> short_distance;
	double short_count;
};

GensouGageView::GensouGageView(void)
{
	member = boost::make_shared<GensouGageViewImpl>();
}

GensouGageView::~GensouGageView(void)
{
}

void GensouGageView::init()
{
	member->long_live_power.Init(sg_pDungeonSystem->pDevice_D3D);
	member->short_live_power.Init(sg_pDungeonSystem->pDevice_D3D);
	{
		double first_half_time = 20;
		double second_half_time = 60;
		double roop_time = 60;
		member->long_size.addCascade(	1,		1.5,	1.5,	first_half_time);
		member->long_size.addCascade(	1.5,	1.5,	1.5,	1,	second_half_time);

		member->long_color.addCascade(cColor(200,200,255,255),cColor(200,255,255,255),cColor(200,255,255,200),roop_time);
		member->long_color.addCascade(cColor(200,255,255,200),cColor(200,255,255,255),cColor(200,255,200,255),roop_time);
		member->long_color.addCascade(cColor(200,255,200,255),cColor(200,255,255,255),cColor(200,200,255,200),roop_time);
		member->long_color.setOutRangeMode(VariationValue_Enum::Mode_Wrap);

		member->long_distance.addCascade(-4,2,2,2,first_half_time);
		member->long_distance.addCascade(2,2,-4,second_half_time);

		member->long_live_power.position.set(780, 100, 0, 0);
		member->long_live_power.Fontindex = 1;
		member->long_live_power.percentSign = true;
		member->long_live_power.distanceQuant = false;
	}

	{
		double first_half_time = 20;
		double second_half_time = 80;
		double roop_time = 60;
		member->short_size.addCascade(1,2,2,first_half_time);
		member->short_size.addCascade(2,2,2,1,second_half_time);

		member->short_color.addCascade(cColor(200,200,255,255),cColor(200,255,255,255),cColor(200,255,255,200),roop_time);
		member->short_color.addCascade(cColor(200,255,255,200),cColor(200,255,255,255),cColor(200,255,200,255),roop_time);
		member->short_color.addCascade(cColor(200,255,200,255),cColor(200,255,255,255),cColor(200,200,255,200),roop_time);
		member->short_color.setOutRangeMode(VariationValue_Enum::Mode_Wrap);
		
		member->short_distance.addCascade(-4,2,2,2,first_half_time);
		member->short_distance.addCascade(2,2,-4,second_half_time);

		member->short_live_power.position.set(780, 180, 0, 0);
		member->short_live_power.Fontindex = 2;
		member->short_live_power.percentSign = true;
		member->short_live_power.plusSign = true;
		member->short_live_power.distanceQuant = false;
	}

	member->long_count = 0;
	member->short_count = 0;
}
void GensouGageView::process()
{
	member->long_count++;
	member->short_count++;
}
void GensouGageView::refreshView()
{
	if (delegate.lock()) {
		double new_short_live_power = delegate.lock()->short_live_power();
		double new_long_live_power = delegate.lock()->long_live_power();

		double old_short_live_power = member->short_live_power.Value_;
		double old_long_live_power = member->long_live_power.Value_;

		if (new_short_live_power > old_short_live_power) {
			member->short_count = 0;
		}

		if (new_long_live_power > old_long_live_power) {
			member->long_count = 0;
		}

		member->short_live_power.Value_ = new_short_live_power;
		member->long_live_power.Value_ = new_long_live_power;
	}
}
void GensouGageView::draw(IDirect3DDevice9 *pDev)
{
	{
		double size = member->long_size.value(member->long_count);
		cColor color = member->long_color.value(member->long_count);
		double distance = member->long_distance.value(member->long_count);

		member->long_live_power.color = color;
		member->long_live_power.FontHeightSize = size*64*0.7;
		member->long_live_power.FontWidthSize = size*32*0.7;
		member->long_live_power.distance = distance;
	}

	{
		double size = member->short_size.value(member->short_count);
		cColor color = member->short_color.value(member->short_count);
		double distance = member->short_distance.value(member->short_count);

		member->short_live_power.color = color;
		member->short_live_power.FontHeightSize = size*64;
		member->short_live_power.FontWidthSize = size*32;
		member->short_live_power.distance = distance;
	}

	member->long_live_power.Draw(pDev);
	member->short_live_power.Draw(pDev);
}