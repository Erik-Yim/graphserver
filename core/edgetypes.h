#ifndef _EDGETYPES_H_
#define _EDGETYPES_H_

#include <stdlib.h>
#include <string.h>
#include "hashtable_gs.h"
#include "hashtable_itr.h"
#include "statetypes.h"

typedef enum {    
  PL_STREET,
  PL_TRIPHOPSCHED,
  PL_TRIPHOP,
  PL_LINK,
  PL_EXTERNVALUE,
  PL_NONE,
  PL_WAIT,
  PL_HEADWAY,
  PL_TRIPBOARD,
  PL_CROSSING,
  PL_ALIGHT,
} edgepayload_t;

//---------------DECLARATIONS FOR STATE CLASS---------------------

typedef struct State {
   long          time;           //seconds since the epoch
   long          weight;
   double        dist_walked;    //meters
   int           num_transfers;
   edgepayload_t prev_edge_type;
   char*         prev_edge_name;
   int           n_agencies;
   ServicePeriod** service_periods;
} State;

State*
stateNew(int numcalendars, long time);

void
stateDestroy( State* this);

State*
stateDup( State* this );

long
stateGetTime( State* this );

long
stateGetWeight( State* this);

double
stateGetDistWalked( State* this );

int
stateGetNumTransfers( State* this );

edgepayload_t
stateGetPrevEdgeType( State* this );

char*
stateGetPrevEdgeName( State* this );

int
stateGetNumAgencies( State* this );

ServicePeriod*
stateServicePeriod( State* this, int agency );

void
stateSetServicePeriod( State* this,  int agency, ServicePeriod* cal );

void
stateSetTime( State* this, long time );

void
stateSetWeight( State* this, long weight );

void
stateSetDistWalked( State* this, double dist );

void
stateSetNumTransfers( State* this, int n);

void
stateSetPrevEdgeType( State* this, edgepayload_t type );

void
stateSetPrevEdgeName( State* this, char* name );

//---------------DECLARATIONS FOR EDGEPAYLOAD CLASS---------------------

typedef struct EdgePayload {
  edgepayload_t type;
  State* (*walk)(struct EdgePayload*, struct State*, int);
  State* (*walkBack)(struct EdgePayload*, struct State*, int);
} EdgePayload;

EdgePayload*
epNew( edgepayload_t type, void* payload );

void
epDestroy( EdgePayload* this );

edgepayload_t
epGetType( EdgePayload* this );

State*
epWalk( EdgePayload* this, State* param, int transferPenalty );

State*
epWalkBack( EdgePayload* this, State* param, int transferPenalty );

EdgePayload*
epCollapse( EdgePayload* this, State* param );

EdgePayload*
epCollapseBack( EdgePayload* this, State* param );

//---------------DECLARATIONS FOR LINK  CLASS---------------------

typedef struct Link {
  edgepayload_t type;
  State* (*walk)(struct EdgePayload*, struct State*, int);
  State* (*walkBack)(struct EdgePayload*, struct State*, int);
    
  char* name;
} Link;

Link*
linkNew();

void
linkDestroy(Link* tokill);

inline State*
linkWalk(EdgePayload* this, State* param, int env);

inline State*
linkWalkBack(EdgePayload* this, State* param, int env);

char*
linkGetName(Link* this);


//---------------DECLARATIONS FOR HEADWAY  CLASS---------------------

typedef struct Headway {
  edgepayload_t type;
  State* (*walk)(struct EdgePayload*, struct State*, int);
  State* (*walkBack)(struct EdgePayload*, struct State*, int);
    
  int begin_time;
  int end_time;
  int wait_period;
  int transit;
  char* trip_id;
  ServiceCalendar* calendar;
  Timezone* timezone;
  int agency;
  ServiceId service_id;
} Headway;

Headway*
headwayNew(int begin_time, int end_time, int wait_period, int transit, char* trip_id, ServiceCalendar* calendar, Timezone* timezone, int agency, ServiceId service_id);

void
headwayDestroy(Headway* tokill);

inline State*
headwayWalk(EdgePayload* this, State* param, int transfer_penalty);

inline State*
headwayWalkBack(EdgePayload* this, State* param, int transfer_penalty);

int
headwayBeginTime(Headway* this);

int
headwayEndTime(Headway* this);

int
headwayWaitPeriod(Headway* this);

int
headwayTransit(Headway* this);

char*
headwayTripId(Headway* this);

ServiceCalendar*
headwayCalendar(Headway* this);

Timezone*
headwayTimezone(Headway* this);

int
headwayAgency(Headway* this);

ServiceId
headwayServiceId(Headway* this);

//---------------DECLARATIONS FOR WAIT CLASS------------------------

typedef struct Wait {
    edgepayload_t type;
    State* (*walk)(struct EdgePayload*, struct State*, int);
    State* (*walkBack)(struct EdgePayload*, struct State*, int);
    
    long end;
    Timezone* timezone;
} Wait;

Wait*
waitNew(long end, Timezone* timezone);

void
waitDestroy(Wait* tokill);

inline State*
waitWalk(EdgePayload* superthis, State* param, int transfer_penalty);

inline State*
waitWalkBack(EdgePayload* superthis, State* param, int transfer_penalty);

long
waitGetEnd(Wait* this);

Timezone*
waitGetTimezone(Wait* this);

//---------------DECLARATIONS FOR STREET  CLASS---------------------

typedef struct Street {
   edgepayload_t type;
   State* (*walk)(struct EdgePayload*, struct State*, int);
   State* (*walkBack)(struct EdgePayload*, struct State*, int);
    
   char* name;
   double length;
} Street;

Street*
streetNew(const char *name, double length);

void
streetDestroy(Street* tokill);

inline State*
streetWalk(EdgePayload* superthis, State* params, int env);

inline State*
streetWalkBack(EdgePayload* superthis, State* params, int env);

char*
streetGetName(Street* this);

double
streetGetLength(Street* this);

//---------------DECLARATIONS FOR TRIPBOARD CLASS------------------------------------------

typedef struct TripBoard {
    edgepayload_t type;
    State* (*walk)(struct EdgePayload*, struct State*, int);
    
    int n;
    int* departs;
    char** trip_ids;
    
    ServiceCalendar* calendar;
    Timezone* timezone;
    int agency;
    ServiceId service_id;
} TripBoard;

TripBoard*
tbNew( ServiceId service_id, ServiceCalendar* calendar, Timezone* timezone, int agency );

void
tbDestroy(TripBoard* this);

ServiceCalendar*
tbGetCalendar( TripBoard* this );

Timezone*
tbGetTimezone( TripBoard* this );

int
tbGetAgency( TripBoard* this );

ServiceId
tbGetServiceId( TripBoard* this );

int
tbGetNumBoardings(TripBoard* this);

void
tbAddBoarding(TripBoard* this, char* trip_id, int depart);

char*
tbGetBoardingTripId(TripBoard* this, int i);

int
tbGetBoardingDepart(TripBoard* this, int i);

int
tbSearchBoardingsList(TripBoard* this, int time);

int
tbGetNextBoardingIndex(TripBoard* this, int time);

inline State*
tbWalk( EdgePayload* superthis, State* params, int transferPenalty );

//---------------DECLARATIONS FOR CROSSING CLASS-------------------------------------------

typedef struct Crossing {
    edgepayload_t type;
    State* (*walk)(struct EdgePayload*, struct State*, int);
    
    int crossing_time;
} Crossing;

Crossing*
crNew( int crossing_time );

void
crDestroy(Crossing* this);

int
crGetCrossingTime(Crossing* this);

inline State*
crWalk( EdgePayload* superthis, State* params, int transferPenalty );

//---------------DECLARATIONS FOR ALIGHT CLASS---------------------------------------------

typedef struct Alight {
  edgepayload_t type;
  State* (*walk)(struct EdgePayload*, struct State*, int);
} Alight;

Alight*
alNew();

void
alDestroy(Alight* this);

inline State*
alWalk(EdgePayload* this, State* params, int transferPenalty);

//---------------DECLARATIONS FOR TRIPHOPSCHEDULE and TRIPHOP  CLASSES---------------------

#define INFINITY 100000000
#define SECONDS_IN_WEEK 604800
#define SECONDS_IN_DAY 86400
#define SECONDS_IN_HOUR 3600
#define SECONDS_IN_MINUTE 60
#define DAYS_IN_WEEK 7

typedef struct TripHopSchedule TripHopSchedule;

typedef struct TripHop {
  edgepayload_t type;
  State* (*walk)(struct EdgePayload*, struct State*, int);
  State* (*walkBack)(struct EdgePayload*, struct State*, int);
    
  int depart;
  int arrive;
  int transit;
  char* trip_id;
  ServiceCalendar* calendar;
  Timezone* timezone;
  int agency;
  ServiceId service_id;
} TripHop;

struct TripHopSchedule {
  edgepayload_t type;
  State* (*walk)(struct EdgePayload*, struct State*, int);
  State* (*walkBack)(struct EdgePayload*, struct State*, int);
    
  int n;
  TripHop** hops;
  ServiceId service_id;
  ServiceCalendar* calendar;
  Timezone* timezone;
  int agency;
};

TripHopSchedule*
thsNew( int *departs, int *arrives, char **trip_ids, int n, ServiceId service_id, ServiceCalendar* calendar, Timezone* timezone, int agency );

void
thsDestroy(TripHopSchedule* this);

TripHop*
triphopNew( int depart, int arrive, char* trip_id, ServiceCalendar* calendar, Timezone* timezone, int agency, ServiceId service_id );

void
triphopDestroy( TripHop* this );

int
triphopDepart( TripHop* this );

int
triphopArrive( TripHop* this );

int
triphopTransit( TripHop* this );

char *
triphopTripId( TripHop* this );

ServiceCalendar*
triphopCalendar( TripHop* this );

Timezone*
triphopTimezone( TripHop* this );

int
triphopAuthority( TripHop* this );

int
triphopServiceId( TripHop* this );


inline State*
thsWalk(EdgePayload* superthis, State* params, int transferPenalty);

inline State*
thsWalkBack(EdgePayload* superthis, State* params, int transferPenalty);

inline State*
triphopWalk( EdgePayload* superthis, State* params, int transferPenalty );

inline State*
triphopWalkBack( EdgePayload* superthis, State* params, int transferPenalty );

inline TripHop*
thsCollapse( TripHopSchedule* this, State* params );

inline TripHop*
thsCollapseBack( TripHopSchedule* this, State* params );

//convert time, N seconds since the epoch, to seconds since midnight within the span of the service day
inline long
thsSecondsSinceMidnight( TripHopSchedule* this, State* param );

inline TripHop*
thsGetNextHop(TripHopSchedule* this, long time);

inline TripHop*
thsGetLastHop(TripHopSchedule* this, long time);

int
thsGetN(TripHopSchedule* this);

ServiceId
thsGetServiceId(TripHopSchedule* this);

TripHop*
thsGetHop(TripHopSchedule* this, int i);

ServiceCalendar*
thsGetCalendar(TripHopSchedule* this );

Timezone*
thsGetTimezone(TripHopSchedule* this );

typedef struct PayloadMethods {
	void (*destroy)(void*);
	State* (*walk)(void*,State*);
	State* (*walkBack)(void*,State*);
	EdgePayload* (*collapse)(void*,State*);
	EdgePayload* (*collapseBack)(void*,State*);
	//char* (*to_str)(void*);
} PayloadMethods;

typedef struct CustomPayload {
  edgepayload_t type;
  void* soul;
  PayloadMethods* methods;
} CustomPayload;

PayloadMethods*
defineCustomPayloadType(void (*destroy)(void*),
						State* (*walk)(void*,State*),
						State* (*walkback)(void*,State*),
						EdgePayload* (*collapse)(void*,State*),
						EdgePayload* (*collapseBack)(void*,State*));


void
undefineCustomPayloadType( PayloadMethods* this );

CustomPayload*
cpNew( void* soul, PayloadMethods* methods );

void
cpDestroy( CustomPayload* this );

void*
cpSoul( CustomPayload* this );

PayloadMethods*
cpMethods( CustomPayload* this );

State*
cpWalk(CustomPayload* this, State* params);

State*
cpWalkBack(CustomPayload* this, State* params);

EdgePayload*
cpCollapse(CustomPayload* this, State* params);

EdgePayload*
cpCollapseBack(CustomPayload* this, State* params);

// ------------ DECLARATIONS FOR GEOM --------------------------

typedef struct Geom {
	char *data;
}Geom;

Geom*
geomNew (char * geomdata);

void
geomDestroy(Geom* this);


//--------------DECLARATIONS FOR COORDINATES---------------------
typedef struct Coordinates {
   long lat;
   long lon;
}Coordinates;

Coordinates*
coordinatesNew(long latitude,long length);

void
coordinatesDestroy(Coordinates* this);

Coordinates*
coordinatesDup(Coordinates* this);

#endif
