/*************************************************************************
	> File Name: haizei_shared_type.h
	> Author: 
	> Mail: 
	> Created Time: 六  4/ 4 20:33:17 2020
 ************************************************************************/

#ifndef _HAIZEI_SHARED_TYPE_H
#define _HAIZEI_SHARED_TYPE_H

#include <memory>

namespace haizei {

class ASTree;
class Parameter;
class IValue;
class IntValue;
class FloatValue;
class StringValue;
class FunctionValue;
class DFA;
class IDFANode;

using SIValue = std::shared_ptr<IValue>;
using SIntValue = std::shared_ptr<IntValue>;
using SFloatValue = std::shared_ptr<FloatValue>;
using SStringValue = std::shared_ptr<StringValue>;
using SFunctionValue = std::shared_ptr<FunctionValue>;
using SParameter = std::shared_ptr<Parameter>;

}

#endif
