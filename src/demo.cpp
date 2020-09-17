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
#include <bimsvg/bimsvg.hpp>

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
    bimsvg<>::Plan bim_plan;
    std::string error_message;
    if (!bimsvg<>::load_from_string(svg_context, bim_plan, error_message, true))
    {
        return 1;
    }

    bim_plan.reset();
    bim_plan.nodes.insert(std::make_pair<>(0, bimsvg<>::Node(0.0, 0.0)));
    bim_plan.nodes.insert(std::make_pair<>(1, bimsvg<>::Node(20.0, 0.0)));
    bim_plan.nodes.insert(std::make_pair<>(2, bimsvg<>::Node(20.0, 20.0)));
    bim_plan.nodes.insert(std::make_pair<>(3, bimsvg<>::Node(0.0, 20.0)));
    bim_plan.nodes.insert(std::make_pair<>(4, bimsvg<>::Node(10.0, 0.0)));
    bim_plan.nodes.insert(std::make_pair<>(5, bimsvg<>::Node(10.0, 10.0)));
    bim_plan.nodes.insert(std::make_pair<>(6, bimsvg<>::Node(10.0, -10.0)));
    bim_plan.nodes.insert(std::make_pair<>(7, bimsvg<>::Node(10.0, -20.0)));
    bim_plan.nodes.insert(std::make_pair<>(8, bimsvg<>::Node(0.0, -20.0)));
    bim_plan.nodes.insert(std::make_pair<>(9, bimsvg<>::Node(0.0, -10.0)));
    bim_plan.nodes.insert(std::make_pair<>(10, bimsvg<>::Node(-10.0, 0.0)));
    bim_plan.nodes.insert(std::make_pair<>(11, bimsvg<>::Node(-10.0, 10.0)));
    bim_plan.walls.insert(std::make_pair<>(0, bimsvg<>::Wall(0, 4)));
    bim_plan.walls.insert(std::make_pair<>(1, bimsvg<>::Wall(4, 5)));
    bim_plan.walls.insert(std::make_pair<>(2, bimsvg<>::Wall(4, 1)));
    bim_plan.walls.insert(std::make_pair<>(3, bimsvg<>::Wall(1, 2)));
    bim_plan.walls.insert(std::make_pair<>(4, bimsvg<>::Wall(2, 3)));
    bim_plan.walls.insert(std::make_pair<>(5, bimsvg<>::Wall(3, 0)));
    bim_plan.walls.insert(std::make_pair<>(6, bimsvg<>::Wall(4, 6)));
    bim_plan.walls.insert(std::make_pair<>(7, bimsvg<>::Wall(6, 7)));
    bim_plan.walls.insert(std::make_pair<>(8, bimsvg<>::Wall(7, 8)));
    bim_plan.walls.insert(std::make_pair<>(9, bimsvg<>::Wall(8, 9)));
    bim_plan.walls.insert(std::make_pair<>(10, bimsvg<>::Wall(9, 6)));
    bim_plan.walls.insert(std::make_pair<>(11, bimsvg<>::Wall(10, 11)));
    bimsvg<>::Area bim_area;
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
    bim_plan.areas.insert(std::make_pair<>(0, bim_area));

    // get the sorted nodes
    std::vector<bimsvg<>::Path> bim_paths;
    if (!bim_plan.calculate_paths(0, bim_paths))
    {
        return 1;
    }
    return 0;
}
