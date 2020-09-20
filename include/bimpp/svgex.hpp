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
#include <bimpp/plan2d.hpp>

#ifndef M_PI
#define M_PI       3.14159265358979323846   // pi
#endif

namespace bimpp
{
    template<typename T = double>
    class svgex
    {
    public:
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
                    plan2d::node<T> new_node(static_cast<T>(json_doc["x"].GetDouble()), static_cast<T>(json_doc["y"].GetDouble()));
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
                    plan2d::wall<T> new_wall;
                    if (json_doc.HasMember("kind"))
                    {
                        new_wall.kind = json_doc["kind"].GetString();
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
                    plan2d::hole<T> new_hole;
                    if (json_doc.HasMember("kind"))
                    {
                        new_hole.kind = json_doc["kind"].GetString();
                    }
                    if (json_doc.HasMember("direction"))
                    {
                        new_hole.direction = json_doc["direction"].GetString();
                    }
                    new_hole.wall_id = json_doc["wall-id"].GetUint64();
                    new_hole.distance = json_doc["distance"].GetDouble();
                    new_hole.width = json_doc["width"].GetDouble();
                    all_holes.insert(std::make_pair<>(bim_id, new_hole));
                }
                else if (json_type == "room")
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
                    plan2d::room<T> new_room;
                    if (json_doc.HasMember("kind"))
                    {
                        new_room.kind = json_doc["kind"].GetString();
                    }
                    for (rapidjson::Value::ConstValueIterator itr = json_wall_ids.Begin(); itr != json_wall_ids.End(); ++itr)
                    {
                        const size_t bim_wall_id = itr->GetUint64();
                        // remove the repeated wall.
                        if (std::find(new_room.wall_ids.cbegin(), new_room.wall_ids.cend(), bim_wall_id) != new_room.wall_ids.cend())
                        {
                            continue;
                        }
                        new_room.wall_ids.push_back(bim_wall_id);
                    }
                    if (new_room.wall_ids.empty())
                    {
                        return;
                    }
                    all_rooms.insert(std::make_pair<>(bim_id, new_room));
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

            bool getHouse(typename plan2d::house<T>& _house, bool _check = false)
            {
                if (_check)
                {
                    for (const std::pair<size_t, typename plan2d::wall<T>>& p_wall : all_walls)
                    {
                        const typename plan2d::wall<T>& bim_wall = p_wall.second;
                        if (!bim_wall.isValid())
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

                    for (const std::pair<size_t, typename plan2d::hole<T>>& p_hole : all_holes)
                    {
                        const typename plan2d::hole<T>& bim_hole = p_hole.second;
                        if (!bim_hole.isValid())
                        {
                            return false;
                        }
                        if (all_walls.find(bim_hole.wall_id) == all_walls.cend())
                        {
                            return false;
                        }
                    }

                    for (const std::pair<size_t, typename plan2d::room<T>>& p_room : all_rooms)
                    {
                        const typename plan2d::room<T>& bim_room = p_room.second;
                        if (bim_room.wall_ids.empty())
                        {
                            return false;
                        }
                        for (size_t wall_id : bim_room.wall_ids)
                        {
                            if (all_walls.find(wall_id) == all_walls.cend())
                            {
                                return false;
                            }
                        }
                    }
                }

                _house.reset();
                _house.nodes = all_nodes;
                _house.walls = all_walls;
                _house.holes = all_holes;
                _house.rooms = all_rooms;
                return true;
            }

        private:
            std::string current_bimpp;
            std::map<size_t, typename plan2d::node<T>> all_nodes;
            std::map<size_t, typename plan2d::wall<T>> all_walls;
            std::map<size_t, typename plan2d::hole<T>> all_holes;
            std::map<size_t, typename plan2d::room<T>> all_rooms;
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
        static bool loadFromString(std::string& _svg, typename plan2d::house<T>& _house, std::string& _error, bool _check = false)
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
                return context.getHouse(_house, _check);
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