/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    ToolPicker.cpp

Abstract:

    

Author / Creation date:

    JulesIMF / 18.10.21

Revision History:

--*/


//
// Includes / usings
//

#include <editor/ToolPicker.h>
#include <common/messages.h>
#include <typeinfo>
#include <cxxabi.h>
using namespace JIMP;

//
// Defines
//

void ToolPicker::setColor(JG::Color color)
{
    // debugMessage("Set new color: (%d, %d, %d)", color.r, color.g, color.b);
    for (auto tool : tools)
        tool->color = color;
}

void ToolPicker::setThickness(int thickness)
{
    // debugMessage("Set new color: (%d, %d, %d)", color.r, color.g, color.b);
    for (auto tool : tools)
        tool->thickness = thickness;
}

Tool* ToolPicker::getTool()
{
    if (tools.empty())
        return nullptr;
    
    return tools.at(index);
}

void ToolPicker::nextTool()
{
    if (tools.empty())
        return;
    
    index = (index + 1) % tools.size();

    int status = 0;
    auto name = abi::__cxa_demangle(typeid(*tools[index]).name(), nullptr, nullptr, &status);
    debugMessage("nextTool: tool set to %s", name);
    free(name);
}

void ToolPicker::prevTool()
{
    if (tools.empty())
        return;

    index = (index + tools.size() - 1) % tools.size();
    
    int status = 0;
    auto name = abi::__cxa_demangle(typeid(*tools[index]).name(), nullptr, nullptr, &status);
    debugMessage("Tool: tool set to %s", name);
    free(name);
}

void ToolPicker::erase(Tool* tool)
{
    auto it = std::find(tools.begin(), tools.end(), tool);
    if (it == tools.end())
        return;
    
    tools.erase(it);
    if (index && index == (int)tools.size())
        index--;
}

void ToolPicker::insert(Tool* tool)
{
    tools.push_back(tool);
}