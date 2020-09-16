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

    std::ifstream ifs(argv[1], std::ifstream::in);
    if (!ifs.good())
    {
        return 1;
    }

    std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

    typedef bimsvg<double> bimsvg_d;
    bimsvg_d::plan plan;
    std::string error_message;
    bimsvg_d::load_from_string(str, plan, error_message, true);
    return 0;
}
