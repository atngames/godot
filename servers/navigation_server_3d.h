/*************************************************************************/
/*  navigation_server_3d.h                                               */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#ifndef NAVIGATION_SERVER_3D_H
#define NAVIGATION_SERVER_3D_H

#include "core/object/class_db.h"
#include "core/templates/rid.h"

#include "scene/3d/navigation_region_3d.h"
#include "servers/navigation/navigation_path_query_parameters_3d.h"
#include "servers/navigation/navigation_path_query_result_3d.h"

/// This server uses the concept of internal mutability.
/// All the constant functions can be called in multithread because internally
/// the server takes care to schedule the functions access.
///
/// Note: All the `set` functions are commands executed during the `sync` phase,
/// don't expect that a change is immediately propagated.

class NavigationServer3D : public Object {
	GDCLASS(NavigationServer3D, Object);

	static NavigationServer3D *singleton;

protected:
	static void _bind_methods();

public:
	/// Thread safe, can be used across many threads.
	static const NavigationServer3D *get_singleton();

	/// MUST be used in single thread!
	static NavigationServer3D *get_singleton_mut();

	virtual TypedArray<RID> get_maps() const = 0;

	/// Create a new map.
	virtual RID map_create() const = 0;

	/// Set map active.
	virtual void map_set_active(RID p_map, bool p_active) const = 0;

	/// Returns true if the map is active.
	virtual bool map_is_active(RID p_map) const = 0;

	/// Set the map UP direction.
	virtual void map_set_up(RID p_map, Vector3 p_up) const = 0;

	/// Returns the map UP direction.
	virtual Vector3 map_get_up(RID p_map) const = 0;

	/// Set the map cell size used to weld the navigation mesh polygons.
	virtual void map_set_cell_size(RID p_map, real_t p_cell_size) const = 0;

	/// Returns the map cell size.
	virtual real_t map_get_cell_size(RID p_map) const = 0;

	/// Set the map edge connection margin used to weld the compatible region edges.
	virtual void map_set_edge_connection_margin(RID p_map, real_t p_connection_margin) const = 0;

	/// Returns the edge connection margin of this map.
	virtual real_t map_get_edge_connection_margin(RID p_map) const = 0;

	/// Set the map link connection radius used to attach links to the nav mesh.
	virtual void map_set_link_connection_radius(RID p_map, real_t p_connection_radius) const = 0;

	/// Returns the link connection radius of this map.
	virtual real_t map_get_link_connection_radius(RID p_map) const = 0;

	/// Returns the navigation path to reach the destination from the origin.
	virtual Vector<Vector3> map_get_path(RID p_map, Vector3 p_origin, Vector3 p_destination, bool p_optimize, uint32_t p_navigation_layers = 1) const = 0;

	virtual Vector3 map_get_closest_point_to_segment(RID p_map, const Vector3 &p_from, const Vector3 &p_to, const bool p_use_collision = false) const = 0;
	virtual Vector3 map_get_closest_point(RID p_map, const Vector3 &p_point) const = 0;
	virtual Vector3 map_get_closest_point_normal(RID p_map, const Vector3 &p_point) const = 0;
	virtual RID map_get_closest_point_owner(RID p_map, const Vector3 &p_point) const = 0;

	virtual TypedArray<RID> map_get_links(RID p_map) const = 0;
	virtual TypedArray<RID> map_get_regions(RID p_map) const = 0;
	virtual TypedArray<RID> map_get_agents(RID p_map) const = 0;

	virtual void map_force_update(RID p_map) = 0;

	/// Creates a new region.
	virtual RID region_create() const = 0;

	/// Set the enter_cost of a region
	virtual void region_set_enter_cost(RID p_region, real_t p_enter_cost) const = 0;
	virtual real_t region_get_enter_cost(RID p_region) const = 0;

	/// Set the travel_cost of a region
	virtual void region_set_travel_cost(RID p_region, real_t p_travel_cost) const = 0;
	virtual real_t region_get_travel_cost(RID p_region) const = 0;

	/// Set the node which manages this region.
	virtual void region_set_owner_id(RID p_region, ObjectID p_owner_id) const = 0;
	virtual ObjectID region_get_owner_id(RID p_region) const = 0;

	virtual bool region_owns_point(RID p_region, const Vector3 &p_point) const = 0;

	/// Set the map of this region.
	virtual void region_set_map(RID p_region, RID p_map) const = 0;
	virtual RID region_get_map(RID p_region) const = 0;

	/// Set the region's layers
	virtual void region_set_navigation_layers(RID p_region, uint32_t p_navigation_layers) const = 0;
	virtual uint32_t region_get_navigation_layers(RID p_region) const = 0;

	/// Set the global transformation of this region.
	virtual void region_set_transform(RID p_region, Transform3D p_transform) const = 0;

	/// Set the navigation mesh of this region.
	virtual void region_set_navmesh(RID p_region, Ref<NavigationMesh> p_nav_mesh) const = 0;

	/// Bake the navigation mesh.
	virtual void region_bake_navmesh(Ref<NavigationMesh> r_mesh, Node *p_node) const = 0;

	/// Get a list of a region's connection to other regions.
	virtual int region_get_connections_count(RID p_region) const = 0;
	virtual Vector3 region_get_connection_pathway_start(RID p_region, int p_connection_id) const = 0;
	virtual Vector3 region_get_connection_pathway_end(RID p_region, int p_connection_id) const = 0;

	/// Creates a new link between locations in the nav map.
	virtual RID link_create() const = 0;

	/// Set the map of this link.
	virtual void link_set_map(RID p_link, RID p_map) const = 0;
	virtual RID link_get_map(RID p_link) const = 0;

	/// Set whether this link travels in both directions.
	virtual void link_set_bidirectional(RID p_link, bool p_bidirectional) const = 0;
	virtual bool link_is_bidirectional(RID p_link) const = 0;

	/// Set the link's layers.
	virtual void link_set_navigation_layers(RID p_link, uint32_t p_navigation_layers) const = 0;
	virtual uint32_t link_get_navigation_layers(RID p_link) const = 0;

	/// Set the start location of the link.
	virtual void link_set_start_location(RID p_link, Vector3 p_location) const = 0;
	virtual Vector3 link_get_start_location(RID p_link) const = 0;

	/// Set the end location of the link.
	virtual void link_set_end_location(RID p_link, Vector3 p_location) const = 0;
	virtual Vector3 link_get_end_location(RID p_link) const = 0;

	/// Set the enter cost of the link.
	virtual void link_set_enter_cost(RID p_link, real_t p_enter_cost) const = 0;
	virtual real_t link_get_enter_cost(RID p_link) const = 0;

	/// Set the travel cost of the link.
	virtual void link_set_travel_cost(RID p_link, real_t p_travel_cost) const = 0;
	virtual real_t link_get_travel_cost(RID p_link) const = 0;

	/// Set the node which manages this link.
	virtual void link_set_owner_id(RID p_link, ObjectID p_owner_id) const = 0;
	virtual ObjectID link_get_owner_id(RID p_link) const = 0;

	/// Creates the agent.
	virtual RID agent_create() const = 0;

	/// Put the agent in the map.
	virtual void agent_set_map(RID p_agent, RID p_map) const = 0;
	virtual RID agent_get_map(RID p_agent) const = 0;

	/// The maximum distance (center point to
	/// center point) to other agents this agent
	/// takes into account in the navigation. The
	/// larger this number, the longer the running
	/// time of the simulation. If the number is too
	/// low, the simulation will not be safe.
	/// Must be non-negative.
	virtual void agent_set_neighbor_distance(RID p_agent, real_t p_distance) const = 0;

	/// The maximum number of other agents this
	/// agent takes into account in the navigation.
	/// The larger this number, the longer the
	/// running time of the simulation. If the
	/// number is too low, the simulation will not
	/// be safe.
	virtual void agent_set_max_neighbors(RID p_agent, int p_count) const = 0;

	/// The minimal amount of time for which this
	/// agent's velocities that are computed by the
	/// simulation are safe with respect to other
	/// agents. The larger this number, the sooner
	/// this agent will respond to the presence of
	/// other agents, but the less freedom this
	/// agent has in choosing its velocities.
	/// Must be positive.
	virtual void agent_set_time_horizon(RID p_agent, real_t p_time) const = 0;

	/// The radius of this agent.
	/// Must be non-negative.
	virtual void agent_set_radius(RID p_agent, real_t p_radius) const = 0;

	/// The maximum speed of this agent.
	/// Must be non-negative.
	virtual void agent_set_max_speed(RID p_agent, real_t p_max_speed) const = 0;

	/// Current velocity of the agent
	virtual void agent_set_velocity(RID p_agent, Vector3 p_velocity) const = 0;

	/// The new target velocity.
	virtual void agent_set_target_velocity(RID p_agent, Vector3 p_velocity) const = 0;

	/// Position of the agent in world space.
	virtual void agent_set_position(RID p_agent, Vector3 p_position) const = 0;

	/// Agent ignore the Y axis and avoid collisions by moving only on the horizontal plane
	virtual void agent_set_ignore_y(RID p_agent, bool p_ignore) const = 0;

	/// Returns true if the map got changed the previous frame.
	virtual bool agent_is_map_changed(RID p_agent) const = 0;

	/// Callback called at the end of the RVO process
	virtual void agent_set_callback(RID p_agent, ObjectID p_object_id, StringName p_method, Variant p_udata = Variant()) const = 0;

	/// Destroy the `RID`
	virtual void free(RID p_object) const = 0;

	/// Control activation of this server.
	virtual void set_active(bool p_active) const = 0;

	/// Process the collision avoidance agents.
	/// The result of this process is needed by the physics server,
	/// so this must be called in the main thread.
	/// Note: This function is not thread safe.
	virtual void process(real_t delta_time) = 0;

	/// Returns a customized navigation path using a query parameters object
	void query_path(const Ref<NavigationPathQueryParameters3D> &p_query_parameters, Ref<NavigationPathQueryResult3D> p_query_result) const;

	virtual NavigationUtilities::PathQueryResult _query_path(const NavigationUtilities::PathQueryParameters &p_parameters) const = 0;

	NavigationServer3D();
	virtual ~NavigationServer3D();

#ifdef DEBUG_ENABLED
private:
	bool debug_enabled = false;
	bool debug_dirty = true;
	void _emit_navigation_debug_changed_signal();

	Color debug_navigation_edge_connection_color = Color(1.0, 0.0, 1.0, 1.0);
	Color debug_navigation_geometry_edge_color = Color(0.5, 1.0, 1.0, 1.0);
	Color debug_navigation_geometry_face_color = Color(0.5, 1.0, 1.0, 0.4);
	Color debug_navigation_geometry_edge_disabled_color = Color(0.5, 0.5, 0.5, 1.0);
	Color debug_navigation_geometry_face_disabled_color = Color(0.5, 0.5, 0.5, 0.4);
	Color debug_navigation_link_connection_color = Color(1.0, 0.5, 1.0, 1.0);
	Color debug_navigation_link_connection_disabled_color = Color(0.5, 0.5, 0.5, 1.0);

	bool debug_navigation_enable_edge_connections = true;
	bool debug_navigation_enable_edge_connections_xray = true;
	bool debug_navigation_enable_edge_lines = true;
	bool debug_navigation_enable_edge_lines_xray = true;
	bool debug_navigation_enable_geometry_face_random_color = true;
	bool debug_navigation_enable_link_connections = true;
	bool debug_navigation_enable_link_connections_xray = true;

	Ref<StandardMaterial3D> debug_navigation_geometry_edge_material;
	Ref<StandardMaterial3D> debug_navigation_geometry_face_material;
	Ref<StandardMaterial3D> debug_navigation_geometry_edge_disabled_material;
	Ref<StandardMaterial3D> debug_navigation_geometry_face_disabled_material;
	Ref<StandardMaterial3D> debug_navigation_edge_connections_material;
	Ref<StandardMaterial3D> debug_navigation_link_connections_material;
	Ref<StandardMaterial3D> debug_navigation_link_connections_disabled_material;

public:
	void set_debug_enabled(bool p_enabled);
	bool get_debug_enabled() const;

	void set_debug_navigation_edge_connection_color(const Color &p_color);
	Color get_debug_navigation_edge_connection_color() const;

	void set_debug_navigation_geometry_edge_color(const Color &p_color);
	Color get_debug_navigation_geometry_edge_color() const;

	void set_debug_navigation_geometry_face_color(const Color &p_color);
	Color get_debug_navigation_geometry_face_color() const;

	void set_debug_navigation_geometry_edge_disabled_color(const Color &p_color);
	Color get_debug_navigation_geometry_edge_disabled_color() const;

	void set_debug_navigation_geometry_face_disabled_color(const Color &p_color);
	Color get_debug_navigation_geometry_face_disabled_color() const;

	void set_debug_navigation_link_connection_color(const Color &p_color);
	Color get_debug_navigation_link_connection_color() const;

	void set_debug_navigation_link_connection_disabled_color(const Color &p_color);
	Color get_debug_navigation_link_connection_disabled_color() const;

	void set_debug_navigation_enable_edge_connections(const bool p_value);
	bool get_debug_navigation_enable_edge_connections() const;

	void set_debug_navigation_enable_edge_connections_xray(const bool p_value);
	bool get_debug_navigation_enable_edge_connections_xray() const;

	void set_debug_navigation_enable_edge_lines(const bool p_value);
	bool get_debug_navigation_enable_edge_lines() const;

	void set_debug_navigation_enable_edge_lines_xray(const bool p_value);
	bool get_debug_navigation_enable_edge_lines_xray() const;

	void set_debug_navigation_enable_geometry_face_random_color(const bool p_value);
	bool get_debug_navigation_enable_geometry_face_random_color() const;

	void set_debug_navigation_enable_link_connections(const bool p_value);
	bool get_debug_navigation_enable_link_connections() const;

	void set_debug_navigation_enable_link_connections_xray(const bool p_value);
	bool get_debug_navigation_enable_link_connections_xray() const;

	Ref<StandardMaterial3D> get_debug_navigation_geometry_face_material();
	Ref<StandardMaterial3D> get_debug_navigation_geometry_edge_material();
	Ref<StandardMaterial3D> get_debug_navigation_geometry_face_disabled_material();
	Ref<StandardMaterial3D> get_debug_navigation_geometry_edge_disabled_material();
	Ref<StandardMaterial3D> get_debug_navigation_edge_connections_material();
	Ref<StandardMaterial3D> get_debug_navigation_link_connections_material();
	Ref<StandardMaterial3D> get_debug_navigation_link_connections_disabled_material();
#endif // DEBUG_ENABLED
};

typedef NavigationServer3D *(*NavigationServer3DCallback)();

/// Manager used for the server singleton registration
class NavigationServer3DManager {
	static NavigationServer3DCallback create_callback;

public:
	static void set_default_server(NavigationServer3DCallback p_callback);
	static NavigationServer3D *new_default_server();
};

#endif // NAVIGATION_SERVER_3D_H
