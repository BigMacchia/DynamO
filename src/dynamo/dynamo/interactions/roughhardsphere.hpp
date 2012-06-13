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

#pragma once

#include <dynamo/interactions/interaction.hpp>
#include <dynamo/interactions/glyphrepresentation.hpp>
#include <dynamo/simulation.hpp>

namespace dynamo {
  class IRoughHardSphere: public Interaction, public GlyphRepresentation
  {
  public:
    template<class T1, class T2, class T3>
    IRoughHardSphere(dynamo::Simulation* tmp, T1 d, T2 e, T3 et, C2Range* nR,
		     std::string name):
      Interaction(tmp, nR),
      _diameter(Sim->_properties.getProperty
		(d, Property::Units::Length())),
      _e(Sim->_properties.getProperty
	 (e, Property::Units::Dimensionless())),
      _et(Sim->_properties.getProperty
	  (et, Property::Units::Dimensionless()))
    { intName = name; }

    IRoughHardSphere(const magnet::xml::Node&, dynamo::Simulation*);

    virtual size_t glyphsPerParticle() const { return 1; }
    virtual Vector getGlyphSize(size_t ID, size_t subID) const;
    virtual Vector getGlyphPosition(size_t ID, size_t subID) const;

    void operator<<(const magnet::xml::Node&);

    virtual double getInternalEnergy() const { return 0.0; }

    virtual void initialise(size_t);

    virtual double maxIntDist() const;

    virtual double getExcludedVolume(size_t) const;

    virtual IntEvent getEvent(const Particle&, const Particle&) const;
 
    virtual void runEvent(Particle&, Particle&, const IntEvent&) const;
   
    virtual void outputXML(magnet::xml::XmlStream&) const;

    virtual void checkOverlaps(const Particle&, const Particle&) const;

  protected:
    shared_ptr<Property> _diameter;
    shared_ptr<Property> _e;
    shared_ptr<Property> _et;
  };
}

