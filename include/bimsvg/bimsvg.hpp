#pragma once

#include <string>
#include <vector>
#include <array>

#include <rapidxml_ns.hpp>
#include <svgpp/svgpp.hpp>
#include <svgpp/policy/xml/rapidxml_ns.hpp>

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
        size_t  start_node_id;
        size_t  end_node_id;
        T       thickness;

        wall(size_t _start_node_id = 0, size_t _end_node_id = 0, size_t _thickness = 0)
            : start_node_id(_start_node_id)
            , end_node_id(_end_node_id)
            , thickness(_thickness)
        {
            //
        }

        bool is_valid() const
        {
            return (start_node_id != end_node_id);
        }
    };

    struct hole
    {
        std::string name;
        std::string type;
        std::string direction;
        size_t      wall_id;
        T           distance;
        T           width;

        hole()
            : name("unknown")
            , wall_id(0)
            , distance(0)
            , width(0)
        {
            //
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
        std::vector<node> nodes;
        std::vector<wall> walls;
        std::vector<area> areas;

        plan()
            : name("unknown")
            , nodes()
            , walls()
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
            areas.clear();
        }

        bool area_wall_ids(size_t _area_id, std::vector<size_t>& _wall_ids) const
        {
            //
            return false;
        }

        bool area_node_ids(size_t _area_id, std::vector<size_t>& _node_ids) const
        {
            //
            return false;
        }
    };

    class bim_context
    {
    public:
        void on_enter_element(svgpp::tag::element::any _any) {}

        void on_exit_element()
        {
            //TODO:
        }

        void on_bimpp(const std::string& _value)
        {
            current_bimpp = _value;
        }

        void path_move_to(double x, double y, svgpp::tag::coordinate::absolute)
        {
            current_node.x = x;
            current_node.y = y;
        }

        void path_line_to(double x, double y, svgpp::tag::coordinate::absolute)
        {
            current_node.x = x;
            current_node.y = y;
        }

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

        bool get_plan(plan& _plan)
        {
            //
            return false;
        }

    private:
        node current_node;
        std::string current_bimpp;
        std::map<size_t, node> all_nodes;
        std::map<size_t, wall> all_walls;
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
    static bool load_from_string(std::string& _svg, plan& _plan)
    {
        bim_context context;
        try
        {
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
        }
        catch (std::exception const& e)
        {
            //
            return false;
        }
        return context.get_plan(_plan);
    }
};