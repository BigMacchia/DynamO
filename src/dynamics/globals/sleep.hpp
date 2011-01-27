/*  DYNAMO:- Event driven molecular dynamics simulator 
    http://www.marcusbannerman.co.uk/dynamo
    Copyright (C) 2011  Marcus N Campbell Bannerman <m.bannerman@gmail.com>
    Copyright (C) 2011  Sebastian Gonzalez <tsuresuregusa@gmail.com>

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

#include "global.hpp"
#include <vector>
#include "../../datatypes/vector.hpp"

class GSleep: public Global
{
public:
  GSleep(const XMLNode&, DYNAMO::SimData*);

  GSleep(DYNAMO::SimData*, const std::string&);
  
  virtual ~GSleep() {}

  virtual Global* Clone() const { return new GSleep(*this); };

  virtual GlobalEvent getEvent(const Particle &) const;

  virtual void runEvent(const Particle&) const;

  virtual void initialise(size_t);

  virtual void operator<<(const XMLNode&);

protected:
  void particlesUpdated(const NEventData&);

  virtual void outputXML(xml::XmlStream&) const;
  
  mutable std::vector<double> sleepTime;
  mutable std::vector<Vector> lastVelocity;
  mutable std::vector<Vector> lastPosition;
};
