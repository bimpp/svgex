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
    bimsvg<>::plan bim_plan;
    std::string error_message;
    if (!bimsvg<>::load_from_string(svg_context, bim_plan, error_message, true))
    {
        return 1;
    }

    // get the sorted nodes
    std::vector<size_t> bim_node_ids;
    if (!bim_plan.area_node_ids(4, bim_node_ids))
    {
        return 1;
    }
    return 0;
}
