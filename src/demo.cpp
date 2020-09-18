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
    bimpp::svgex<>::Plan bim_plan;
    std::string error_message;
    if (!bimpp::svgex<>::load_from_string(svg_context, bim_plan, error_message, true))
    {
        return 1;
    }

    /*bim_plan.reset();
    bim_plan.nodes.insert(std::make_pair<>(0, svgex<>::Node(0.0, 0.0)));
    bim_plan.nodes.insert(std::make_pair<>(1, svgex<>::Node(20.0, 0.0)));
    bim_plan.nodes.insert(std::make_pair<>(2, svgex<>::Node(20.0, 20.0)));
    bim_plan.nodes.insert(std::make_pair<>(3, svgex<>::Node(0.0, 20.0)));
    bim_plan.nodes.insert(std::make_pair<>(4, svgex<>::Node(10.0, 0.0)));
    bim_plan.nodes.insert(std::make_pair<>(5, svgex<>::Node(10.0, 10.0)));
    bim_plan.nodes.insert(std::make_pair<>(6, svgex<>::Node(10.0, -10.0)));
    bim_plan.nodes.insert(std::make_pair<>(7, svgex<>::Node(10.0, -20.0)));
    bim_plan.nodes.insert(std::make_pair<>(8, svgex<>::Node(0.0, -20.0)));
    bim_plan.nodes.insert(std::make_pair<>(9, svgex<>::Node(0.0, -10.0)));
    bim_plan.nodes.insert(std::make_pair<>(10, svgex<>::Node(-10.0, 0.0)));
    bim_plan.nodes.insert(std::make_pair<>(11, svgex<>::Node(-10.0, 10.0)));
    bim_plan.walls.insert(std::make_pair<>(0, svgex<>::Wall(0, 4)));
    bim_plan.walls.insert(std::make_pair<>(1, svgex<>::Wall(4, 5)));
    bim_plan.walls.insert(std::make_pair<>(2, svgex<>::Wall(4, 1)));
    bim_plan.walls.insert(std::make_pair<>(3, svgex<>::Wall(1, 2)));
    bim_plan.walls.insert(std::make_pair<>(4, svgex<>::Wall(2, 3)));
    bim_plan.walls.insert(std::make_pair<>(5, svgex<>::Wall(3, 0)));
    bim_plan.walls.insert(std::make_pair<>(6, svgex<>::Wall(4, 6)));
    bim_plan.walls.insert(std::make_pair<>(7, svgex<>::Wall(6, 7)));
    bim_plan.walls.insert(std::make_pair<>(8, svgex<>::Wall(7, 8)));
    bim_plan.walls.insert(std::make_pair<>(9, svgex<>::Wall(8, 9)));
    bim_plan.walls.insert(std::make_pair<>(10, svgex<>::Wall(9, 6)));
    bim_plan.walls.insert(std::make_pair<>(11, svgex<>::Wall(10, 11)));
    svgex<>::Area bim_area;
    bim_area.wall_ids.push_back(0);
    bim_area.wall_ids.push_back(1);
    bim_area.wall_ids.push_back(2);
    bim_area.wall_ids.push_back(3);
    bim_area.wall_ids.push_back(4);
    bim_area.wall_ids.push_back(5);
    bim_area.wall_ids.push_back(6);
    bim_area.wall_ids.push_back(7);
    bim_area.wall_ids.push_back(8);
    bim_area.wall_ids.push_back(9);
    bim_area.wall_ids.push_back(10);
    bim_area.wall_ids.push_back(11);
    bim_plan.areas.insert(std::make_pair<>(0, bim_area));*/

    // get the sorted nodes
    std::vector<bimpp::svgex<>::Path> bim_paths;
    if (!bim_plan.calculate_paths(4, bim_paths))
    {
        return 1;
    }
    return 0;
}
