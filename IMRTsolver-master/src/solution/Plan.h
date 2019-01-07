/*
 * Schedule.h
 *
 *  Created on: 7 may. 2018
 *      Author: iaraya
 */

#ifndef PLAN_H_
#define PLAN_H_

#include "EvaluationFunction.h"
#include "Station.h"

namespace imrt {


/* An IMRT plan
 *
 * It consists in a list of stations, each station corresponds to an
 * angle, and a set of apertures (or a matrix of intensities)
 */

class Plan {
public:
	Plan(EvaluationFunction &ev);

  Plan(EvaluationFunction &ev, vector<double> w, vector<double> Zmin, vector<double> Zmax);

  Plan(vector<double> w, vector<double> Zmin, vector<double> Zmax, Collimator& collimator, vector<Volume>& volumes,
       int max_apertures, int max_intensity, int initial_intensity, int step_intensity=2, int open_apertures=-1, int setup=6);

  Plan(const Plan &p);

	virtual ~Plan() {};

	void newCopy(Plan& p);

	// Adds a new station to the plan
	void add_station(Station& s);

	double eval(vector<double>& w, vector<double>& Zmin, vector<double>& Zmax);

	double eval();

	double incremental_eval (Station& station, list< pair< int, double > >& diff);

	// This function assumes that there are no changes made without evaluation
	// performed with eval or incrementalEval
	double getEvaluation();

	const list<Station*>& get_stations() const;

	void write_open_beamlets();

	set < pair< pair<double,bool>, pair<Station*, int> >,
       std::greater < pair< pair<double,bool>, pair<Station*, int> > > >
	  best_beamlets(int n, int nv, int mode=0);

	virtual pair<bool, pair<Station*, int>> getLSBeamlet(int bsize, int vsize){
		  auto sb=ev.best_beamlets(*this, bsize, vsize);
		  auto it=sb.begin();
		  std::advance(it,rand()%sb.size());
		  return make_pair(it->first.second, it->second);
	}

	void undoLast();

	//Lepi's version
	void undoLast2();

	void printIntensity(int station);

	void writeIntensities(string file, int n);

	EvaluationFunction* getEvaluationFunction(){
		return &ev;
	}

	Station* get_station(int i){
	    list<Station*>::iterator s= stations.begin();
	    advance(s,i);
	    return *s;
	}

private:
	//The list of stations
	//list<Station> real_stations;
	list<Station*> stations;

  int n_stations;

  Station* last_changed;
	list< pair< int, double > > last_diff;

	EvaluationFunction ev;

	vector<double> w;
	vector<double> Zmin;
	vector<double> Zmax;

	double evaluation_fx;
};

} /* namespace imrt */

#endif /* PLAN_H_ */
