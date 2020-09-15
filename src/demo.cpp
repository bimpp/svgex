#include <bimsvg/bimsvg.h>

#include <fstream>

#include <rapidxml_ns.hpp>
#include <svgpp/policy/xml/rapidxml_ns.hpp>

typedef rapidxml_ns::xml_node<> const* xml_element_t;

class Context
{
public:
    void on_enter_element(svgpp::tag::element::any _any)
    {
        //
    }

    void on_exit_element()
    {
        //
    }

    void on_bimpp(const std::string& _value) const
    {
        //
    }

    void path_move_to(double x, double y, svgpp::tag::coordinate::absolute _coord)
    {
        //
    }

    void path_line_to(double x, double y, svgpp::tag::coordinate::absolute _coord)
    {
        //
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

    void path_close_subpath()
    {
        //
    }

    void path_exit()
    {
        //
    }
};

typedef
boost::mpl::set<
    // SVG Structural Elements
    svgpp::tag::element::svg,
    // SVG Shape Elements
    svgpp::tag::element::circle,
    svgpp::tag::element::line,
    svgpp::tag::element::path
>::type processed_elements_t;

typedef boost::mpl::set <
    boost::mpl::pair<svgpp::tag::element::circle, svgpp::tag::attribute::cx>,
    boost::mpl::pair<svgpp::tag::element::circle, svgpp::tag::attribute::cy>,
    boost::mpl::pair<svgpp::tag::element::circle, svgpp::tag::attribute::r>,
    boost::mpl::pair<svgpp::tag::element::line, svgpp::tag::attribute::x1>,
    boost::mpl::pair<svgpp::tag::element::line, svgpp::tag::attribute::y1>,
    boost::mpl::pair<svgpp::tag::element::line, svgpp::tag::attribute::x2>,
    boost::mpl::pair<svgpp::tag::element::line, svgpp::tag::attribute::y2>,
    boost::mpl::pair<svgpp::tag::element::path, svgpp::tag::attribute::d>
> ::type processed_attributes_by_element_t;

struct custom_error_policy : svgpp::policy::error::raise_exception<Context>
{
    template<class XMLAttribute, class AttributeName>
    static bool unknown_attribute(Context const& _context,
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
            throw svgpp::unknown_attribute_error(name)
            << boost::error_info<svgpp::tag::error_info::xml_attribute, XMLAttribute>(attribute);
        else
            return true;
    }

    template<class XMLAttribute, class AttributeName>
    static bool unknown_attribute(Context const&,
        XMLAttribute const& attribute,
        AttributeName const&,
        BOOST_SCOPED_ENUM(svgpp::detail::namespace_id) namespace_id,
        svgpp::tag::source::attribute,
        typename boost::disable_if<typename svgpp::detail::is_char_range<AttributeName>::type>::type* = NULL)
    {
        const std::string attribute_name = attribute->name();
        if (attribute_name == "bimpp")
        {
            return true;
        }
        const std::string attribute_value = attribute->value();
        if (namespace_id == svgpp::detail::namespace_id::svg)
            throw svgpp::unknown_attribute_error()
            << boost::error_info<svgpp::tag::error_info::xml_attribute, XMLAttribute>(attribute);
        else
            return true;
    }

    template<class XMLAttribute, class AttributeName>
    SVGPP_NORETURN static bool unknown_attribute(Context const&,
        XMLAttribute const& attribute,
        AttributeName const& name,
        svgpp::tag::source::css,
        typename boost::enable_if<typename svgpp::detail::is_char_range<AttributeName>::type>::type* = NULL)
    {
        throw svgpp::unknown_css_property_error(name)
            << boost::error_info<svgpp::tag::error_info::xml_attribute, XMLAttribute>(attribute);
    }

    template<class XMLAttribute, class AttributeName>
    SVGPP_NORETURN static bool unknown_attribute(Context const&,
        XMLAttribute const& attribute,
        AttributeName const&,
        svgpp::tag::source::css,
        typename boost::disable_if<typename svgpp::detail::is_char_range<AttributeName>::type>::type* = NULL)
    {
        throw svgpp::unknown_css_property_error()
            << boost::error_info<svgpp::tag::error_info::xml_attribute, XMLAttribute>(attribute);
    }
};

void loadSvg(xml_element_t xml_root_element, Context& context)
{
    svgpp::document_traversal<
        //svgpp::xml_element_policy<xml_element_t>,
        svgpp::error_policy<custom_error_policy>,
        svgpp::processed_elements<processed_elements_t>,
        svgpp::processed_attributes<processed_attributes_by_element_t>
    >::load_document(xml_root_element, context);
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        return 1;
    }

    std::ifstream ifs(argv[1], std::ifstream::in);
    if (!ifs.good())
    {
        return 1;
    }

    std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

    try
    {
        rapidxml_ns::xml_document<> doc;
        doc.parse<0>(&str[0]);
        Context context;
        if (rapidxml_ns::xml_node<>* svg_element = doc.first_node("svg"))
        {
            loadSvg(svg_element, context);
        }

        typedef bimsvg<double> bimsvg_d;
        bimsvg_d::plan plan;
        bimsvg_d::load_from_string(str, plan);
    }
    catch (std::exception const& e)
    {
        //
        return 1;
    }
    return 0;
}
