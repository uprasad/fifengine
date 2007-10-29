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

#ifndef FIFE_RENDERERBASE_H
#define FIFE_RENDERERBASE_H

// Standard C++ library includes
#include <list>

// 3rd party library includes

// FIFE includes
// These includes are split up in two parts, separated by one empty line
// First block: files included from the FIFE root src directory
// Second block: files included from the same folder

namespace FIFE {
	class Camera;
	class Layer;
	class Elevation;
	class Instance;
	
	class RendererBase;
	/** RendererListener allows reaction to changes in renderer
	 *  Having this implemented via callback mechanism should speed up the rendering a bit
	 *  (e.g. no need to sort pipeline order per each frame)
	 */
	class RendererListener {
	public:
		virtual ~RendererListener() {}
		
		/** Renderer's pipeline position has been changed
		 */
		virtual void onRendererPipelinePositionChanged(RendererBase* renderer) = 0;
		
		/** Renderer is enabled / disabled
		 */
		virtual void onRendererEnabledChanged(RendererBase* renderer) = 0;
	};
	
	
	// use some big value, so that non-positioned renderers show on top
	const int DEFAULT_RENDERER_POSITION = 1000;
	
	/** Base class for all view renderers
	 * View renderer renders one aspect of the view shown on screen
	 */
	class RendererBase {
	public:
		/** Constructor
		 */
		RendererBase();
		
		/** Destructor
		 */
		virtual ~RendererBase() {};
		
		/** This method is called by the view to ask renderer to draw its rendering aspect based on
		 * given parameters. Renderers receive non-clipped instance stack since there is no
		 * way to know which information is relevant for the renderer. E.g. effect renderer
		 * might need to know offscreen instance locations to be able to draw radiation coming from
		 * some instance not visible on the screen.
		 *
		 * @param cam camera view to draw
		 * @param cam current layer to be rendered
		 * @param instances instances on the current layer
		 * @ see setPipelinePosition
		 */
		virtual void render(Camera* cam, Layer* layer, std::vector<Instance*>& instances) = 0;
		
		/** Name of the renderer
		 */
		virtual std::string getName() = 0;
		
		/** Gets renderer position in the rendering pipeline
		 */
		int getPipelinePosition() const { return m_pipeline_position; }
		
		/** Sets renderer position in the rendering pipeline
		 *  Pipeline position defines in which order view calls the renderers when update occurs
		 *  Note that renderers are called once per rendered layer, thus to update the
		 *  whole screen, renderer might receive multiple calls
		 */
		void setPipelinePosition(int position);
		
		/** Enables renderer
		 */
		virtual void setEnabled(bool enabled);
		
		/** Is renderer enabled
		 */
		bool isEnabled() const { return m_enabled; }
		
		/** Sets listener for renderer
		 */
		void setRendererListener(RendererListener* listener) { m_listener = listener; }
		
		/** Adds active layer to renderer. Only active layers are rendered
		 */
		void addActiveLayer(Layer* layer);
		
		/** Removes active layer from renderer.
		 */
		void removeActiveLayer(Layer* layer);
		
		/** Clears all active layers from renderer
		 */
		void clearActiveLayers();
		
		/** Activates all layers from given elevation
		 */
		void activateAllLayers(Elevation* elevation);
		
		/** Returns if given layer is currently activated
		 */
		bool isActivedLayer(Layer* layer);
		
	
	protected:
		std::list<Layer*> m_active_layers;
	
	private:
		bool m_enabled;
		int m_pipeline_position;
		RendererListener* m_listener;
	};
}

#endif