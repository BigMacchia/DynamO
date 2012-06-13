/*  dynamo:- Event driven molecular dynamics simulator 
    http://www.dynamomd.org
    Copyright (C) 2011  Marcus N Campbell Bannerman <m.bannerman@gmail.com>

    This program is free software: you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    version 3 as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <dynamo/globals/ParabolaSentinel.hpp>
#include <dynamo/globals/globEvent.hpp>
#include <dynamo/NparticleEventData.hpp>
#include <dynamo/simulation.hpp>
#include <dynamo/liouvillean/liouvillean.hpp>
#include <dynamo/outputplugins/outputplugin.hpp>
#include <dynamo/schedulers/scheduler.hpp>
#include <magnet/xmlreader.hpp>

#ifdef DYNAMO_DEBUG 
#include <boost/math/special_functions/fpclassify.hpp>
#endif

namespace dynamo {

  GParabolaSentinel::GParabolaSentinel(dynamo::Simulation* nSim, const std::string& name):
    Global(nSim, "ParabolaSentinel")
  {
    globName = name;
    dout << "ParabolaSentinel Loaded" << std::endl;
  }

  void 
  GParabolaSentinel::initialise(size_t nID)
  {
    ID=nID;
  }

  GlobalEvent 
  GParabolaSentinel::getEvent(const Particle& part) const
  {
    Sim->liouvillean->updateParticle(Sim->particleList[part.getID()]);

    return GlobalEvent(part, Sim->liouvillean
		       ->getParabolaSentinelTime(part), 
		       VIRTUAL_PARABOLA, *this);
  }

  void 
  GParabolaSentinel::runEvent(Particle& part, const double) const
  {
    Sim->liouvillean->updateParticle(part);

    GlobalEvent iEvent(getEvent(part));

    if (iEvent.getdt() == HUGE_VAL)
      {
	//We've numerically drifted slightly passed the parabola, so
	//just reschedule the particles events, no need to enforce anything
	Sim->ptrScheduler->fullUpdate(part);
	return;
      }

#ifdef DYNAMO_DEBUG 
    if (boost::math::isnan(iEvent.getdt()))
      M_throw() << "A NAN Interaction collision time has been found when recalculating this global"
		<< iEvent.stringData(Sim);
#endif

    Sim->dSysTime += iEvent.getdt();
    
    Sim->ptrScheduler->stream(iEvent.getdt());
  
    Sim->stream(iEvent.getdt());

    Sim->liouvillean->enforceParabola(part);
  
#ifdef DYNAMO_DEBUG
    iEvent.addTime(Sim->freestreamAcc);
  
    Sim->freestreamAcc = 0;

    NEventData EDat(ParticleEventData(part, Sim->species[part], VIRTUAL_PARABOLA));

    Sim->signalParticleUpdate(EDat);

    BOOST_FOREACH(shared_ptr<OutputPlugin> & Ptr, Sim->outputPlugins)
      Ptr->eventUpdate(iEvent, EDat);
#else
    Sim->freestreamAcc += iEvent.getdt();
#endif

    Sim->ptrScheduler->fullUpdate(part);
  }
}
