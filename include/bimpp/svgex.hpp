/*
 * The MIT License (MIT)
 * Copyright © 2020 BIM++
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the “Software”),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
#pragma once

#include <string>
#include <vector>
#include <array>
#include <algorithm>

#include <rapidxml_ns.hpp>
#include <svgpp/svgpp.hpp>
#include <svgpp/policy/xml/rapidxml_ns.hpp>
#include <rapidjson/document.h>

#ifndef M_PI
#define M_PI       3.14159265358979323846   // pi
#endif

namespace bimpp
{
    /*!
     * main
     */
    template<typename T = double>
    class svgex
    {
    public:
        /*!
         * This is a 2d point
         */
        struct Node
        {
            T x;    //< value in x-axis
            T y;    //< value in y-axis

            /*!
             * A constructor
             * /param _x value in x-axis
             * /param _y value in y-axis
             */
            Node(T _x = 0, T _y = 0)
                : x(_x)
                , y(_y)
            {
                //
            }
        };

        struct NextNode
        {
            size_t  wall_id;
            bool    wall_inversed;
            size_t  node_id;
            bool    used;

            NextNode()
                : wall_id(0)
                , wall_inversed(false)
                , node_id(0)
                , used(false)
            {}
        };

        struct Wall
        {
            std::string name;
            size_t  start_node_id;
            size_t  end_node_id;
            T       thickness;

            Wall(size_t _start_node_id = 0, size_t _end_node_id = 0, T _thickness = 0)
                : start_node_id(_start_node_id)
                , end_node_id(_end_node_id)
                , thickness(_thickness)
            {
                //
            }

            bool is_valid() const
            {
                return (start_node_id != end_node_id && thickness > 0);
            }
        };

        struct Hole
        {
            std::string name;
            std::string kind;
            std::string direction;
            size_t      wall_id;
            T           width;
            T           distance;

            Hole()
                : name("unknown")
                , kind("unknown")
                , direction("none")
                , wall_id(0)
                , width(0)
                , distance(0)
            {
                //
            }

            bool is_valid() const
            {
                return (width > 0 && distance > 0);
            }
        };

        struct Area
        {
            std::string name;
            std::vector<size_t> wall_ids;

            Area()
                : name("unknown")
                , wall_ids()
            {
                //
            }
        };

        struct Path
        {
            struct WallEx
            {
                size_t wall_id;
                bool inversed;

                WallEx()
                    : wall_id(0)
                    , inversed(false)
                {
                    //
                }
            };

            size_t area_id;
            bool inside;
            std::vector<WallEx> walls;

            Path()
                : area_id(0)
                , inside(false)
                , walls()
            {
                //
            }
        };

        struct Plan
        {
            std::string name;
            std::map<size_t, Node> nodes;
            std::map<size_t, Wall> walls;
            std::map<size_t, Hole> holes;
            std::map<size_t, Area> areas;

            Plan()
                : name("unknown")
                , nodes()
                , walls()
                , holes()
                , areas()
            {
                //
            }

            bool is_valid() const
            {
                return (!nodes.empty() && !walls.empty() && !areas.empty());
            }

            void reset()
            {
                name = "unknown";
                nodes.clear();
                walls.clear();
                holes.clear();
                areas.clear();
            }

            bool calculate_paths(size_t _area_id, std::vector<Path>& _paths) const
            {
                const typename std::map<size_t, Area>::const_iterator cit_found_area = areas.find(_area_id);
                if (cit_found_area == areas.cend())
                {
                    return false;
                }
                std::map<size_t, std::vector<NextNode>> bim_node_2_next_nodes;

                const Area& bim_area = cit_found_area->second;
                for (const size_t wall_id : bim_area.wall_ids)
                {
                    const Wall& bim_wall = walls.find(wall_id)->second;
                    NextNode bim_next_node;
                    bim_next_node.wall_id = wall_id;
                    bim_next_node.wall_inversed = false;
                    bim_next_node.node_id = bim_wall.end_node_id;
                    bim_node_2_next_nodes[bim_wall.start_node_id].push_back(bim_next_node);
                    bim_next_node.wall_inversed = true;
                    bim_next_node.node_id = bim_wall.start_node_id;
                    bim_node_2_next_nodes[bim_wall.end_node_id].push_back(bim_next_node);
                }

                if (bim_node_2_next_nodes.empty())
                {
                    return false;
                }

                auto calculate_sin_angle_ex = [](const Node& _o, const Node& _a, const Node& _b) {
                    Node line_a(_a.x - _o.x, _a.y - _o.y);
                    T len_a = sqrt(line_a.x * line_a.x + line_a.y * line_a.y);
                    if (len_a != 0)
                    {
                        line_a.x /= len_a;
                        line_a.y /= len_a;
                    }
                    Node line_b(_b.x - _o.x, _b.y - _o.y);
                    T len_b = sqrt(line_b.x * line_b.x + line_b.y * line_b.y);
                    if (len_b != 0)
                    {
                        line_b.x /= len_b;
                        line_b.y /= len_b;
                    }
                    T sin_res = line_a.x * line_b.y - line_a.y * line_b.x;
                    T cos_res = line_a.x * line_b.x + line_a.y * line_b.y;
                    T res = sin_res;
                    if (cos_res < static_cast<T>(0))
                    {
                        if (sin_res >= static_cast<T>(0))
                        {
                            res = static_cast<T>(2) - res;
                        }
                        else
                        {
                            res = static_cast<T>(-2) + res;
                        }
                    }
                    if (res < static_cast<T>(0))
                    {
                        res = static_cast<T>(4) + res;
                    }
                    return res;
                };

                /// compute the closed path
                while (!bim_node_2_next_nodes.empty())
                {
                    Path bim_closed_path;
                    bim_closed_path.area_id = _area_id;
                    bool bim_path_is_closed = false;

                    size_t bim_start_node_id = bim_node_2_next_nodes.cbegin()->first;
                    size_t bim_last_node_id = bim_start_node_id;
                    size_t bim_first_wall_start_node_id = 0;
                    size_t bim_first_wall_end_node_id = 0;
                    typename Path::WallEx bim_first_wall_ex;
                    while (true)
                    {
                        const Node& bim_start_node = nodes.find(bim_start_node_id)->second;
                        std::vector<NextNode>& bim_next_nodes = bim_node_2_next_nodes.find(bim_start_node_id)->second;
                        if (bim_next_nodes.empty())
                        {
                            break;
                        }
                        if (bim_start_node_id == bim_last_node_id)
                        {
                            NextNode& bim_next_node = bim_next_nodes[0];
                            bim_start_node_id = bim_next_node.node_id;
                            bim_first_wall_ex.wall_id = bim_next_node.wall_id;
                            bim_first_wall_ex.inversed = bim_next_node.wall_inversed;
                        }
                        else
                        {
                            const Node& bim_last_node = nodes.find(bim_last_node_id)->second;
                            std::map<T, size_t> sin_angle_ex_2_index;
                            for (size_t i = 0; i < bim_next_nodes.size(); ++i)
                            {
                                const NextNode& bim_next_node = bim_next_nodes[i];
                                if (bim_next_node.used) continue;
                                const Node& bim_node = nodes.find(bim_next_node.node_id)->second;
                                T sin_angle_ex = calculate_sin_angle_ex(bim_start_node, bim_last_node, bim_node);
                                sin_angle_ex_2_index.insert(std::make_pair<>(sin_angle_ex, i));
                            }
                            if (sin_angle_ex_2_index.empty())
                            {
                                break;
                            }

                            typename std::map<T, size_t>::const_iterator sin_angle_ex_2_index_last = sin_angle_ex_2_index.cend();
                            --sin_angle_ex_2_index_last;
                            NextNode& bim_next_node = bim_next_nodes[sin_angle_ex_2_index_last->second];
                            bim_next_node.used = true;

                            typename Path::WallEx wall_ex;
                            wall_ex.wall_id = bim_next_node.wall_id;
                            wall_ex.inversed = bim_next_node.wall_inversed;
                            bim_closed_path.walls.push_back(wall_ex);

                            if (bim_first_wall_ex.wall_id == bim_next_node.wall_id
                                && bim_first_wall_ex.inversed == bim_next_node.wall_inversed)
                            {
                                bim_path_is_closed = true;
                                break;
                            }

                            bim_last_node_id = bim_start_node_id;
                            bim_start_node_id = bim_next_node.node_id;

                            if (bim_start_node_id == bim_last_node_id)
                            {
                                break;
                            }
                        }
                    }

                    if (bim_path_is_closed)
                    {
                        _paths.push_back(bim_closed_path);
                    }

                    for (typename std::map<size_t, std::vector<NextNode>>::iterator it_m = bim_node_2_next_nodes.begin(); it_m != bim_node_2_next_nodes.end();)
                    {
                        std::vector<NextNode>& bim_next_nodes = it_m->second;
                        for (typename std::vector<NextNode>::iterator it_v = bim_next_nodes.begin(); it_v != bim_next_nodes.end();)
                        {
                            if (it_v->used)
                            {
                                it_v = bim_next_nodes.erase(it_v);
                            }
                            else
                            {
                                ++it_v;
                            }
                        }

                        if (it_m->second.empty())
                        {
                            it_m = bim_node_2_next_nodes.erase(it_m);
                        }
                        else
                        {
                            ++it_m;
                        }
                    }
                }

                /// decide wheather the closed path is inside or outside
                for (Path& closed_path : _paths)
                {
                    //
                }

                return !_paths.empty();
            }
        };

        class BimPPContext
        {
        public:
            void on_enter_element(svgpp::tag::element::any _any) {}

            void on_exit_element()
            {
                if (current_bimpp.empty())
                {
                    return;
                }

                const std::string bimpp_data = current_bimpp;
                current_bimpp.clear();

                rapidjson::Document json_doc;
                json_doc.Parse(bimpp_data.c_str());
                if (!json_doc.HasMember("type")
                    || !json_doc.HasMember("id"))
                {
                    return;
                }
                const rapidjson::Value& json_type = json_doc["type"];
                const rapidjson::Value& json_id = json_doc["id"];
                if (!json_type.IsString()
                    || !json_id.IsUint64())
                {
                    return;
                }
                const size_t bim_id = json_id.GetUint64();
                if (json_type == "node")
                {
                    if (!json_doc.HasMember("x")
                        || !json_doc.HasMember("y"))
                    {
                        return;
                    }
                    Node new_node;
                    new_node.x = json_doc["x"].GetDouble();
                    new_node.y = json_doc["y"].GetDouble();
                    all_nodes.insert(std::make_pair<>(bim_id, new_node));
                }
                else if (json_type == "wall")
                {
                    if (!json_doc.HasMember("start-node-id")
                        || !json_doc.HasMember("end-node-id")
                        || !json_doc.HasMember("thickness"))
                    {
                        return;
                    }
                    Wall new_wall;
                    if (json_doc.HasMember("name"))
                    {
                        new_wall.name = json_doc["name"].GetString();
                    }
                    new_wall.start_node_id = json_doc["start-node-id"].GetUint64();
                    new_wall.end_node_id = json_doc["end-node-id"].GetUint64();
                    new_wall.thickness = json_doc["thickness"].GetDouble();
                    all_walls.insert(std::make_pair<>(bim_id, new_wall));
                }
                else if (json_type == "hole")
                {
                    if (!json_doc.HasMember("wall-id")
                        || !json_doc.HasMember("distance")
                        || !json_doc.HasMember("width"))
                    {
                        return;
                    }
                    Hole new_hole;
                    if (json_doc.HasMember("name"))
                    {
                        new_hole.name = json_doc["name"].GetString();
                    }
                    if (json_doc.HasMember("kind"))
                    {
                        new_hole.kind = json_doc["kind"].GetString();
                    }
                    if (json_doc.HasMember("direction"))
                    {
                        new_hole.direction = json_doc["direction"].GetString();
                    }
                    new_hole.wall_id = json_doc["wall-id"].GetUint64();
                    new_hole.width = json_doc["width"].GetDouble();
                    new_hole.distance = json_doc["distance"].GetDouble();
                    all_holes.insert(std::make_pair<>(bim_id, new_hole));
                }
                else if (json_type == "area")
                {
                    if (!json_doc.HasMember("wall-ids"))
                    {
                        return;
                    }
                    const rapidjson::Value& json_wall_ids = json_doc["wall-ids"];
                    if (!json_wall_ids.IsArray())
                    {
                        return;
                    }
                    Area new_area;
                    if (json_doc.HasMember("name"))
                    {
                        new_area.name = json_doc["name"].GetString();
                    }
                    for (rapidjson::Value::ConstValueIterator itr = json_wall_ids.Begin(); itr != json_wall_ids.End(); ++itr)
                    {
                        const size_t bim_wall_id = itr->GetUint64();
                        // remove the repeated wall.
                        if (std::find(new_area.wall_ids.cbegin(), new_area.wall_ids.cend(), bim_wall_id) != new_area.wall_ids.cend())
                        {
                            continue;
                        }
                        new_area.wall_ids.push_back(bim_wall_id);
                    }
                    if (new_area.wall_ids.empty())
                    {
                        return;
                    }
                    all_areas.insert(std::make_pair<>(bim_id, new_area));
                }
            }

            void on_bimpp(const std::string& _value)
            {
                current_bimpp = _value;
                // use `"` to replace `'`
                for (char& c : current_bimpp)
                {
                    if (c != '\'') continue;
                    c = '\"';
                }
            }

            void path_move_to(double x, double y, svgpp::tag::coordinate::absolute) {}

            void path_line_to(double x, double y, svgpp::tag::coordinate::absolute) {}

            void path_cubic_bezier_to(
                double x1, double y1,
                double x2, double y2,
                double x, double y,
                svgpp::tag::coordinate::absolute)
            {}

            void path_quadratic_bezier_to(
                double x1, double y1,
                double x, double y,
                svgpp::tag::coordinate::absolute)
            {}

            void path_elliptical_arc_to(
                double rx, double ry, double x_axis_rotation,
                bool large_arc_flag, bool sweep_flag,
                double x, double y,
                svgpp::tag::coordinate::absolute)
            {}

            void path_close_subpath() {}

            void path_exit() {}

            bool get_plan(Plan& _plan, bool _check = false)
            {
                if (_check)
                {
                    for (const std::pair<size_t, Wall>& p_wall : all_walls)
                    {
                        const Wall& bim_wall = p_wall.second;
                        if (!bim_wall.is_valid())
                        {
                            return false;
                        }
                        if (all_nodes.find(bim_wall.start_node_id) == all_nodes.cend())
                        {
                            return false;
                        }
                        if (all_nodes.find(bim_wall.end_node_id) == all_nodes.cend())
                        {
                            return false;
                        }
                    }

                    for (const std::pair<size_t, Hole>& p_hole : all_holes)
                    {
                        const Hole& bim_hole = p_hole.second;
                        if (!bim_hole.is_valid())
                        {
                            return false;
                        }
                        if (all_walls.find(bim_hole.wall_id) == all_walls.cend())
                        {
                            return false;
                        }
                    }

                    for (const std::pair<size_t, Area>& p_area : all_areas)
                    {
                        const Area& bim_area = p_area.second;
                        if (bim_area.wall_ids.empty())
                        {
                            return false;
                        }
                        for (size_t wall_id : bim_area.wall_ids)
                        {
                            if (all_walls.find(wall_id) == all_walls.cend())
                            {
                                return false;
                            }
                        }
                    }
                }

                _plan.reset();
                _plan.nodes = all_nodes;
                _plan.walls = all_walls;
                _plan.holes = all_holes;
                _plan.areas = all_areas;
                return true;
            }

        private:
            std::string current_bimpp;
            std::map<size_t, Node> all_nodes;
            std::map<size_t, Wall> all_walls;
            std::map<size_t, Hole> all_holes;
            std::map<size_t, Area> all_areas;
        };

    private:
        typedef rapidxml_ns::xml_node<> const* xml_element_t;

        struct BimPPErrorPolicy : svgpp::policy::error::raise_exception<BimPPContext>
        {
            template<class XMLAttribute, class AttributeName>
            static bool unknown_attribute(BimPPContext& _context,
                XMLAttribute const& _attribute,
                AttributeName const& name,
                BOOST_SCOPED_ENUM(svgpp::detail::namespace_id) namespace_id,
                svgpp::tag::source::attribute,
                typename boost::enable_if<typename svgpp::detail::is_char_range<AttributeName>::type>::type* = NULL)
            {
                const std::string attribute_name = _attribute->name();
                if (attribute_name == "bimpp")
                {
                    const std::string attribute_value = _attribute->value();
                    _context.on_bimpp(attribute_value);
                    return true;
                }
                if (namespace_id == svgpp::detail::namespace_id::svg)
                    throw svgpp::unknown_attribute_error(name) << boost::error_info<svgpp::tag::error_info::xml_attribute, XMLAttribute>(_attribute);
                else
                    return true;
            }

            template<class XMLAttribute, class AttributeName>
            static bool unknown_attribute(BimPPContext& _context,
                XMLAttribute const& _attribute,
                AttributeName const&,
                BOOST_SCOPED_ENUM(svgpp::detail::namespace_id) namespace_id,
                svgpp::tag::source::attribute,
                typename boost::disable_if<typename svgpp::detail::is_char_range<AttributeName>::type>::type* = NULL)
            {
                const std::string attribute_name = _attribute->name();
                if (attribute_name == "bimpp")
                {
                    const std::string attribute_value = _attribute->value();
                    _context.on_bimpp(attribute_value);
                    return true;
                }
                const std::string attribute_value = _attribute->value();
                if (namespace_id == svgpp::detail::namespace_id::svg)
                    throw svgpp::unknown_attribute_error() << boost::error_info<svgpp::tag::error_info::xml_attribute, XMLAttribute>(_attribute);
                else
                    return true;
            }

            template<class XMLAttribute, class AttributeName>
            SVGPP_NORETURN static bool unknown_attribute(BimPPContext const&,
                XMLAttribute const& attribute,
                AttributeName const& name,
                svgpp::tag::source::css,
                typename boost::enable_if<typename svgpp::detail::is_char_range<AttributeName>::type>::type* = NULL)
            {
                throw svgpp::unknown_css_property_error(name) << boost::error_info<svgpp::tag::error_info::xml_attribute, XMLAttribute>(attribute);
            }

            template<class XMLAttribute, class AttributeName>
            SVGPP_NORETURN static bool unknown_attribute(BimPPContext const&,
                XMLAttribute const& attribute,
                AttributeName const&,
                svgpp::tag::source::css,
                typename boost::disable_if<typename svgpp::detail::is_char_range<AttributeName>::type>::type* = NULL)
            {
                throw svgpp::unknown_css_property_error() << boost::error_info<svgpp::tag::error_info::xml_attribute, XMLAttribute>(attribute);
            }
        };

        typedef boost::mpl::set<
            svgpp::tag::element::svg,
            svgpp::tag::element::circle,
            svgpp::tag::element::line,
            svgpp::tag::element::path
        >::type TBimPPProcessedElements;

        typedef boost::mpl::set<
            boost::mpl::pair<svgpp::tag::element::circle, svgpp::tag::attribute::cx>,
            boost::mpl::pair<svgpp::tag::element::circle, svgpp::tag::attribute::cy>,
            boost::mpl::pair<svgpp::tag::element::circle, svgpp::tag::attribute::r>,
            boost::mpl::pair<svgpp::tag::element::line, svgpp::tag::attribute::x1>,
            boost::mpl::pair<svgpp::tag::element::line, svgpp::tag::attribute::y1>,
            boost::mpl::pair<svgpp::tag::element::line, svgpp::tag::attribute::x2>,
            boost::mpl::pair<svgpp::tag::element::line, svgpp::tag::attribute::y2>,
            boost::mpl::pair<svgpp::tag::element::path, svgpp::tag::attribute::d>
        >::type TBimPPProcessedAttributesByElement;

    public:
        static bool load_from_string(std::string& _svg, Plan& _plan, std::string& _error, bool _check = false)
        {
            try
            {
                BimPPContext context;
                rapidxml_ns::xml_document<> xml_doc;
                xml_doc.parse<0>(&_svg[0]);
                rapidxml_ns::xml_node<>* xml_svg_element = xml_doc.first_node("svg");
                if (!xml_svg_element)
                {
                    return false;
                }
                svgpp::document_traversal<
                    svgpp::error_policy<BimPPErrorPolicy>,
                    svgpp::processed_elements<TBimPPProcessedElements>,
                    svgpp::processed_attributes<TBimPPProcessedAttributesByElement>
                >::load_document(xml_svg_element, context);
                return context.get_plan(_plan, _check);
            }
            catch (std::exception const& e)
            {
                _error = e.what();
                return false;
            }
            _error = "unknown reason";
            return false;
        }
    };

}