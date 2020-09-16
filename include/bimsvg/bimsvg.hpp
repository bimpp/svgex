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

#include <rapidxml_ns.hpp>
#include <svgpp/svgpp.hpp>
#include <svgpp/policy/xml/rapidxml_ns.hpp>
#include <rapidjson/document.h>

template<typename T>
class bimsvg
{
public:
    union node
    {
        struct
        {
            T x;
            T y;
        };
        std::array<T, 2> d;

        node(T _x = 0, T _y = 0)
            : x(_x)
            , y(_y)
        {
            //
        }
    };

    struct wall
    {
        std::string name;
        size_t  start_node_id;
        size_t  end_node_id;
        T       thickness;

        wall(size_t _start_node_id = 0, size_t _end_node_id = 0, T _thickness = 0)
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

    struct hole
    {
        std::string name;
        std::string kind;
        std::string direction;
        size_t      wall_id;
        T           width;
        T           distance;

        hole()
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

    struct area
    {
        std::string name;
        std::vector<size_t> wall_ids;

        area()
            : name("unknown")
            , wall_ids()
        {
            //
        }
    };

    struct plan
    {
        std::string name;
        std::map<size_t, node> nodes;
        std::map<size_t, wall> walls;
        std::map<size_t, hole> holes;
        std::map<size_t, area> areas;

        plan()
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
            //
            return !areas.empty();
        }

        void reset()
        {
            name = "unknown";
            nodes.clear();
            walls.clear();
            holes.clear();
            areas.clear();
        }

        bool area_wall_ids(size_t _area_id, std::vector<size_t>& _wall_ids) const
        {
            if (areas.find(_area_id) == areas.cend())
            {
                return false;
            }
            const area& bim_area = areas[_area_id];
            //TODO:
            return true;
        }

        bool area_node_ids(size_t _area_id, std::vector<size_t>& _node_ids) const
        {
            std::vector<size_t> wall_ids;
            if (!area_wall_ids(_area_id, wall_ids))
            {
                return false;
            }
            //TODO:
            return true;
        }
    };

    class bim_context
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
                node new_node;
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
                wall new_wall;
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
                hole new_hole;
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
                area new_area;
                if (json_doc.HasMember("name"))
                {
                    new_area.name = json_doc["name"].GetString();
                }
                for (rapidjson::Value::ConstValueIterator itr = json_wall_ids.Begin(); itr != json_wall_ids.End(); ++itr)
                {
                    new_area.wall_ids.push_back(itr->GetUint64());
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

        bool get_plan(plan& _plan, bool _check = false)
        {
            if (_check)
            {
                for (const std::pair<size_t, wall>& p_wall : all_walls)
                {
                    const wall& bim_wall = p_wall.second;
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

                for (const std::pair<size_t, hole>& p_hole : all_holes)
                {
                    const hole& bim_hole = p_hole.second;
                    if (!bim_hole.is_valid())
                    {
                        return false;
                    }
                    if (all_walls.find(bim_hole.wall_id) == all_walls.cend())
                    {
                        return false;
                    }
                }

                for (const std::pair<size_t, area>& p_area : all_areas)
                {
                    const area& bim_area = p_area.second;
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
        std::map<size_t, node> all_nodes;
        std::map<size_t, wall> all_walls;
        std::map<size_t, hole> all_holes;
        std::map<size_t, area> all_areas;
    };

private:
    typedef rapidxml_ns::xml_node<> const* xml_element_t;

    struct bim_error_policy : svgpp::policy::error::raise_exception<bim_context>
    {
        template<class XMLAttribute, class AttributeName>
        static bool unknown_attribute(bim_context& _context,
            XMLAttribute const& attribute,
            AttributeName const& name,
            BOOST_SCOPED_ENUM(svgpp::detail::namespace_id) namespace_id,
            svgpp::tag::source::attribute,
            typename boost::enable_if<typename svgpp::detail::is_char_range<AttributeName>::type>::type* = NULL)
        {
            const std::string attribute_name = attribute->name();
            if (attribute_name == "bimpp")
            {
                const std::string attribute_value = attribute->value();
                _context.on_bimpp(attribute_value);
                return true;
            }
            if (namespace_id == svgpp::detail::namespace_id::svg)
                throw svgpp::unknown_attribute_error(name) << boost::error_info<svgpp::tag::error_info::xml_attribute, XMLAttribute>(attribute);
            else
                return true;
        }

        template<class XMLAttribute, class AttributeName>
        static bool unknown_attribute(bim_context&,
            XMLAttribute const& attribute,
            AttributeName const&,
            BOOST_SCOPED_ENUM(svgpp::detail::namespace_id) namespace_id,
            svgpp::tag::source::attribute,
            typename boost::disable_if<typename svgpp::detail::is_char_range<AttributeName>::type>::type* = NULL)
        {
            const std::string attribute_name = attribute->name();
            if (attribute_name == "bimpp")
            {
                const std::string attribute_value = attribute->value();
                _context.on_bimpp(attribute_value);
                return true;
            }
            const std::string attribute_value = attribute->value();
            if (namespace_id == svgpp::detail::namespace_id::svg)
                throw svgpp::unknown_attribute_error() << boost::error_info<svgpp::tag::error_info::xml_attribute, XMLAttribute>(attribute);
            else
                return true;
        }

        template<class XMLAttribute, class AttributeName>
        SVGPP_NORETURN static bool unknown_attribute(bim_context const&,
            XMLAttribute const& attribute,
            AttributeName const& name,
            svgpp::tag::source::css,
            typename boost::enable_if<typename svgpp::detail::is_char_range<AttributeName>::type>::type* = NULL)
        {
            throw svgpp::unknown_css_property_error(name) << boost::error_info<svgpp::tag::error_info::xml_attribute, XMLAttribute>(attribute);
        }

        template<class XMLAttribute, class AttributeName>
        SVGPP_NORETURN static bool unknown_attribute(bim_context const&,
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
    >::type bim_processed_elements_t;

    typedef boost::mpl::set<
        boost::mpl::pair<svgpp::tag::element::circle, svgpp::tag::attribute::cx>,
        boost::mpl::pair<svgpp::tag::element::circle, svgpp::tag::attribute::cy>,
        boost::mpl::pair<svgpp::tag::element::circle, svgpp::tag::attribute::r>,
        boost::mpl::pair<svgpp::tag::element::line, svgpp::tag::attribute::x1>,
        boost::mpl::pair<svgpp::tag::element::line, svgpp::tag::attribute::y1>,
        boost::mpl::pair<svgpp::tag::element::line, svgpp::tag::attribute::x2>,
        boost::mpl::pair<svgpp::tag::element::line, svgpp::tag::attribute::y2>,
        boost::mpl::pair<svgpp::tag::element::path, svgpp::tag::attribute::d>
    >::type bim_processed_attributes_by_element_t;

public:
    static bool load_from_string(std::string& _svg, plan& _plan, std::string& _error, bool _check = false)
    {
        try
        {
            bim_context context;
            rapidxml_ns::xml_document<> xml_doc;
            xml_doc.parse<0>(&_svg[0]);
            rapidxml_ns::xml_node<>* xml_svg_element = xml_doc.first_node("svg");
            if (!xml_svg_element)
            {
                return false;
            }
            svgpp::document_traversal<
                svgpp::error_policy<bim_error_policy>,
                svgpp::processed_elements<bim_processed_elements_t>,
                svgpp::processed_attributes<bim_processed_attributes_by_element_t>
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