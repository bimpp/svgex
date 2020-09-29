Welcome to SVGEX's documentation!
===================================

* Document_
* Repository_

Features
========

* Header-Only
* Parse the result of PlanReader_

Usage
=====

It depends svgpp_ (needs boost_), rapidxml_ns_, rapidjson_, and plan2d_.

.. doxygenfunction:: bimpp::svgex::loader::load

.. code-block:: cpp

    // Load the svg file to a string `svg_context`
    std::ifstream svg_ifs("file.svg", std::ifstream::in);
    if (!svg_ifs.good())
    {
        // The svg file is invalid!
        return;
    }
    std::string svg_context((std::istreambuf_iterator<char>(svg_ifs)), std::istreambuf_iterator<char>());

    // Declare a house - `bim_house` and a string - `error_message`
    bimpp::svgex::loader<>::house_type bim_house;
    std::string error_message;
    // Parse the svg to the bim data
    if (!bimpp::svgex::loader<>::load(svg_context, bim_house, error_message, true))
    {
        // The svg context has error?
        return;
    }

License
=======

.. code-block:: c
   :linenos:

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

.. _Document: https://bimpp.io/docs/svgex
.. _Repository: https://gitee.com/bimpp/svgex
.. _PlanReader: https://bimpp.io/docs/planreader
.. _boost: https://www.boost.org/
.. _rapidxml_ns: https://github.com/svgpp/rapidxml_ns
.. _svgpp: https://github.com/svgpp/svgpp
.. _rapidjson: https://github.com/Tencent/rapidjson
.. _plan2d: https://gitee.com/bimpp/plan2d
