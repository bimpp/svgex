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
#include <bimpp/svgex.hpp>

#include <boost/polygon/polygon.hpp>

#include <fstream>

int main(int argc, char* argv[])
{

    if (argc != 2)
    {
        return 1;
    }

    // load the svg file
    std::ifstream ifs(argv[1], std::ifstream::in);
    if (!ifs.good())
    {
        return 1;
    }

    std::string svg_context((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

    // parse the svg to the bim
    bimpp::data<>::Plan bim_plan;
    std::string error_message;
    if (!bimpp::svgex<>::load_from_string(svg_context, bim_plan, error_message, true))
    {
        return 1;
    }

    bim_plan.reset();
    bim_plan.nodes.insert(std::make_pair<>(0, bimpp::data<>::Node(0.0, 0.0)));
    bim_plan.nodes.insert(std::make_pair<>(1, bimpp::data<>::Node(20.0, 0.0)));
    bim_plan.nodes.insert(std::make_pair<>(2, bimpp::data<>::Node(20.0, 20.0)));
    bim_plan.nodes.insert(std::make_pair<>(3, bimpp::data<>::Node(0.0, 20.0)));
    bim_plan.nodes.insert(std::make_pair<>(4, bimpp::data<>::Node(10.0, 0.0)));
    bim_plan.nodes.insert(std::make_pair<>(5, bimpp::data<>::Node(10.0, 10.0)));
    bim_plan.nodes.insert(std::make_pair<>(6, bimpp::data<>::Node(10.0, -10.0)));
    bim_plan.nodes.insert(std::make_pair<>(7, bimpp::data<>::Node(10.0, -20.0)));
    bim_plan.nodes.insert(std::make_pair<>(8, bimpp::data<>::Node(0.0, -20.0)));
    bim_plan.nodes.insert(std::make_pair<>(9, bimpp::data<>::Node(0.0, -10.0)));
    bim_plan.nodes.insert(std::make_pair<>(10, bimpp::data<>::Node(-10.0, 0.0)));
    bim_plan.nodes.insert(std::make_pair<>(11, bimpp::data<>::Node(-10.0, 10.0)));
    bim_plan.walls.insert(std::make_pair<>(0, bimpp::data<>::Wall(0, 4)));
    bim_plan.walls.insert(std::make_pair<>(1, bimpp::data<>::Wall(4, 5)));
    bim_plan.walls.insert(std::make_pair<>(2, bimpp::data<>::Wall(4, 1)));
    bim_plan.walls.insert(std::make_pair<>(3, bimpp::data<>::Wall(1, 2)));
    bim_plan.walls.insert(std::make_pair<>(4, bimpp::data<>::Wall(2, 3)));
    bim_plan.walls.insert(std::make_pair<>(5, bimpp::data<>::Wall(3, 0)));
    bim_plan.walls.insert(std::make_pair<>(6, bimpp::data<>::Wall(4, 6)));
    bim_plan.walls.insert(std::make_pair<>(7, bimpp::data<>::Wall(6, 7)));
    bim_plan.walls.insert(std::make_pair<>(8, bimpp::data<>::Wall(7, 8)));
    bim_plan.walls.insert(std::make_pair<>(9, bimpp::data<>::Wall(8, 9)));
    bim_plan.walls.insert(std::make_pair<>(10, bimpp::data<>::Wall(9, 6)));
    bim_plan.walls.insert(std::make_pair<>(11, bimpp::data<>::Wall(10, 11)));
    bimpp::data<>::Area bim_area;
    bim_area.wall_ids = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
    bim_plan.areas.insert(std::make_pair<>(4, bim_area));

    // get the sorted nodes
    std::vector<bimpp::algorithm<>::Path> bim_paths;
    if (!bimpp::algorithm<>::calculate_paths(bim_plan, 4, bim_paths))
    {
        return 1;
    }
    return 0;
}
