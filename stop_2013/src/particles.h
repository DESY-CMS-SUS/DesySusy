#ifndef PARTICLES_H
#define PARTICLES_H

/*
 *  particles.h
 *  
 *
 *  Created on 2/11/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

//namespace topness {
const double PI=3.141592653589793238; // PI 

// particle classes call following functions 


void error(string s) 
{ 
	throw runtime_error(s); 
} 

// Lorentz inner product 
template <class T>
Doub lp(const T x, const T y) 
{    // Lorentz product . assume x, y Lorentz vectors ordered as (px,py,pz,E) etc. Initialize with 
	// metric g==(-1,-1,-1,1)  
	
	double g1[]={-1.,-1.,-1.,1.};
	int gsize=sizeof(g1)/sizeof(double); 
	vector<double> g(gsize);
	g.insert(g.begin(),g1,g1+gsize);
	double s=0;
	for (int j=0;j<(x.size());j++)   
	{s=s+ x[j]*g[j]*y[j]; }        
	return s;
};

// eucliean vector dot product 
template <class T>
double dp(const T x, const T y) 
{    // dot product . assume x, y Lorentz vectors ordered as (px,py,pz,E) etc. Initialize with 
	// metric g==(-1,-1,-1,1)  
	
	int lx=x.size();
	int ly=y.size();
	if (lx != ly) {throw("Taking dot product of vectors of unequal size");}
	
	double g1[]={-1.,-1.,-1.,1.};
	int gsize=sizeof(g1)/sizeof(double); 
	vector<double> g(gsize);
	g.insert(g.begin(),g1,g1+gsize);
	double s=0;
	for (int j=0;j<(x.size()-1);j++)   // only sum over spatial components 
	{s=s+ x[j]*g[j]*y[j]; }        
	return -s;  // - to obtain (+)\vec{x} \cdot \vec{y}
};

double sign ( double x) 
{ if (x>0) 
	return 1.0; 
	else return -1.0; }



// header file containing EventT Record, EventT, and Particle classes 

// begin classes 

// work on three Classes: event record, event, particle

// the EventTRecord class is a vector of the events generated 

// the EventT class is a vector of the Particles in each event 

// the Particle class is a vector of the 4-momentum and particle ID of the particle


class ParticleT { // record of 4-momenta of particle and its id 
	
private: 
	int particleid; 
	vector<double> p4;  
	
	
public: 
	ParticleT(const int id, const vector<double> p); // initialize particle using 4-vector and id.
	
	ParticleT(const int id, const double px, const double py, const double pz, const double E); // initialize particle using momentum components and id.
	
	ParticleT () {}; 
	
	int id() {return particleid;} 
	
	int size() {return p4.size();}  // gives size of p4. size of Particle is ++1. 
	
	vector<double> p() {return p4;}
	
	double p(int j) {if ((j<=p4.size()) && (j>0)) return p4[j-1];
		else error("Call to p vector out of range");}  // NOTE numbering! p(1)=p4[0]=px, .. p(4) = p4[3]=E. 
	
	double m() 
	{double sum=lp(p4,p4);  
		if (sum>0) 	
			return sqrt(sum);
		else return -sqrt(-sum);}
	
	double m2() 
	{double sum=lp(p4,p4); 
		return sum;}
	
	double Cos() 
	{return p4[p4.size()-2]/sqrt(dp(p4,p4));}
	
	double rap() 
	{ double cs=p4[p4.size()-2]/sqrt(dp(p4,p4));
		return 0.5 * log((1+cs)/(1-cs));} 
	
	double y() 
	{return 0.5* log((p4[p4.size()-1]+p4[p4.size()-2])/(p4[p4.size()-1]-p4[p4.size()-2]));}
	
	double pT()
	{double pT2=0; 
		for (int i=0; i<(p4.size()-2); i++) {pT2=pT2+p4[i]*p4[i]; }  
		return sqrt(pT2);}
	
	double phi() 
	{double temp; 
		if (p4[0]>0.) {temp=atan(p4[1]/p4[0]);}
		else temp=sign(p4[1])*PI+ atan(p4[1]/p4[0]);
		if (temp<0) 
		{temp = temp + 2* PI;}
		return temp;}
	
	double ET()   // transverse energy 
	{double ET2; 
		ET2=pow(pT(),2)+m2(); 
		return sqrt(ET2); 
	}
	
};

ParticleT::ParticleT(const int ppid,const vector<double> pp4 ): p4(pp4), particleid(ppid) 
{ 
}

ParticleT::ParticleT(const int ppid, const double ppx, const double ppy, const double ppz, const double EE) 
{
	p4.push_back(ppx); 
    p4.push_back(ppy); 
	p4.push_back(ppz); 
	p4.push_back(EE); 
	particleid = ppid; 
}

bool myfunction_pTsort(ParticleT p1, ParticleT p2)   // compares pT of p1 and p2
{
	double pT1=p1.pT(); 
	double pT2=p2.pT(); 
	return (pT1>pT2); 
}

double mT(ParticleT p1, ParticleT p2)   // computes transverse mass of p1 and p2. returns units of mass 
{ 
	
	double transversemass_sq; 
	
	transversemass_sq=pow(p1.ET()+p2.ET(),2)-pow(p1.p(1)+p2.p(1),2)-pow(p1.p(2)+p2.p(2),2);
	
	if (transversemass_sq <0.) 
	{
		transversemass_sq=0.; 
	}
	
	return sqrt(transversemass_sq);
	
}	

ParticleT operator+ (ParticleT &a, ParticleT &b)
{ 
	int la=a.size();  // should give 4
	int lb=b.size();
	if ((la != lb) && (la !=4)) {throw("Adding momentum vectors of unequal size or not of length 4");}
	ParticleT temp(-99,a.p(1)+b.p(1),a.p(2)+b.p(2),a.p(3)+b.p(3),a.p(4)+b.p(4));   // not physical to add id's; id==-99
	
	return temp;
}

ParticleT operator- (ParticleT &a, ParticleT &b)
{ 
	int la=a.size();  // should give 4
	int lb=b.size();
	if ((la != lb) && (la !=4)) {throw("Adding momentum vectors of unequal size or not of length 4");}
	
	ParticleT temp(-99,a.p(1)-b.p(1),a.p(2)-b.p(2),a.p(3)-b.p(3),a.p(4)-b.p(4));   // not physical to add id's; id==-99
	
	return temp;
}

ParticleT operator- (ParticleT &a)
{ 
	int la=a.size();  // should give 4
	
	ParticleT temp(a.id(),-a.p(1),-a.p(2),-a.p(3),-a.p(4));   	
	return temp;
}




class EventT {  // record of particles in the event. Note that EventT is secretly a vector of particles, so we can make use of standard c++ library functions 
	
private: vector<ParticleT> event;
	
public: 	 
	
	EventT(){}; 
	EventT(const vector<ParticleT>); 
	int size() {return event.size();} 
	ParticleT particle(int i) {if ((i<=event.size()) && (i>0)) 
		return event[i-1];                   // NOTE: first particle is at i=1!
		else error("Call to event out of range");}  
	
	void push_back(const ParticleT particle); 
	
	// define begin() and end() iterators
	vector<ParticleT>::iterator begin(); 
	vector<ParticleT>::iterator end(); 
	vector<ParticleT>::iterator it; 
	void erase(int p); 
	
	
}; 


EventT::EventT(const vector<ParticleT> eevent): event(eevent) 
{
}

void EventT::push_back(const ParticleT particle) 
{
	event.push_back(particle); 
}




vector<ParticleT>::iterator EventT::begin() 
{ return event.begin(); 
}

vector<ParticleT>::iterator EventT::end() 
{ return event.end(); 
}

void EventT::erase(int p) 
{ event.erase(event.begin()+p); 
}





void test_pT_ordering(EventT & subevents) 
{
	
	// first, print out particles before pT ordering....
	
	cout <<"Particles before pT ordering...." << endl; 
	
	for (int k=1;k<=subevents.size();k++) 
	{cout << subevents.particle(k).id() << " " << subevents.particle(k).p(1) << " " << subevents.particle(k).p(2)<< " " 
		<<subevents.particle(k).p(3) << " " << subevents.particle(k).p(4)<< " " << subevents.particle(k).pT() << endl;
	}; 
	
	sort(subevents.begin(),subevents.end(),myfunction_pTsort);
	
	cout << "...and after pT ordering...."<< endl; 
	
	for (int k=1;k<=subevents.size();k++) 
	{cout << subevents.particle(k).id() << " " << subevents.particle(k).p(1) << " " << subevents.particle(k).p(2)<< " " 
		<<subevents.particle(k).p(3) << " " << subevents.particle(k).p(4)<< " " << subevents.particle(k).pT() << endl;
	}; 
	
}

EventT myfunction_join(EventT event1, EventT event2) // returns a new vector of Particles that is simply a vector of the particles in event1 and event2
{
	EventT newevent;
	
	for (int i=1; i<=event1.size(); i++) 
	{newevent.push_back(event1.particle(i));} 
	
	for (int j=1; j<=event2.size(); j++) 
	{newevent.push_back(event2.particle(j));}
	
	return newevent; 
}


class EventTRecord { // record of events 
	
	
private: vector<EventT> eventrec;
	
public: 
	EventTRecord(const vector<EventT>);   
	EventTRecord(){};	
	int size() {return eventrec.size();}
	EventT e(int i) {if ((i<=eventrec.size()) && (i>0)) 
		return eventrec[i-1];
		else throw("Call to event record out of range");}    // NOTE: first event is at i=1.! 
	
	void push_back(EventT event);
};

EventTRecord::EventTRecord(const vector<EventT> eeventrec): eventrec(eeventrec) 
{
}

void EventTRecord:: push_back (const EventT event)
{
	eventrec.push_back(event); 
}



//}
#endif
