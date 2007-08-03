/***************************************************************************
 *   Copyright (C) 2005-2007 by the FIFE Team                              *
 *   fife-public@lists.sourceforge.net                                     *
 *   This file is part of FIFE.                                            *
 *                                                                         *
 *   FIFE is free software; you can redistribute it and/or modify          *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA              *
 ***************************************************************************/

// Standard C++ library includes

// 3rd party library includes
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

// FIFE includes
// These includes are split up in two parts, separated by one empty line
// First block: files included from the FIFE root src directory
// Second block: files included from the same folder
#include "util/exception.h"
#include "util/purge.h"

#include "layer.h"
#include "instance.h"
#include "elevation.h"

namespace FIFE { namespace model {

	Layer::Layer(GeometryType* geometry)
		: AttributedClass("map_Layer"),
		m_geometry(geometry),
 		m_shift() {

		m_objects_visibility = true;

		// set default attributes
		set<std::string>("_OVERLAY_IMAGE","content/gfx/tiles/outlines/tile_outline_fallout.png");

	}

	Layer::~Layer() {
		purge(m_objects);
	}

	Elevation* Layer::getElevation() {
		return m_elevation;
	}

	GeometryType* Layer::getGeometryType() {
		return m_geometry;
	}

	void Layer::setShift(const Point& shift) {
		m_shift = shift;
	}

	const Point& Layer::getShift() const {
		return m_shift;
	}
	
	bool Layer::hasInstances() const {
		return !m_objects.empty();
	}

	Instance* Layer::addInstance(Object* object, const Point& p) {

		Location l;
		l.elevation = getElevation();
		l.layer = this;
		l.position = p;

		Instance* instance = new Instance(object, l);
		m_objects.push_back(instance);
		return instance;
	}

	void Layer::removeInstance(Instance* instance) {

		std::vector<Instance*>::iterator it = m_objects.begin();
		for(; it != m_objects.end(); ++it) {
			if(*it == instance) {
				delete *it;
				m_objects.erase(it);
				break;
			}
		}
	}

	const std::vector<Instance*>& Layer::getInstances() {
		return m_objects;
	}

	template<typename T>
	std::vector<Instance*> Layer::getInstances(const std::string& field, const T& value) {
		std::vector<Instance*> matches;

		std::vector<Instance*>::iterator it = m_objects.begin();
		for(; it != m_objects.end(); ++it) {
			if((*it)->get<T>(field, value))
				matches.push_back(*it);	
		}

		return matches;
	}

	void Layer::setInstancesVisible(bool vis) {
		m_objects_visibility = vis;
	}
	void Layer::toggleInstancesVisible() {
		m_objects_visibility = !m_objects_visibility;
	}
	bool Layer::areInstancesVisible() const {
		return m_objects_visibility;
	}

} } // FIFE::model
