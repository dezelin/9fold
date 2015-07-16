//
// 9fold - 3D graphics and animation software
// Copyright (C) 2015  Aleksandar Dezelin <dezelin@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "v8scriptingengine.h"

#include <v8.h>

namespace _9fold
{
namespace scripting
{
namespace engine
{

using namespace v8;

class V8ScriptingEngine::V8ScriptingEnginePrivate
{
public:
    V8ScriptingEnginePrivate()
    {
        V8::Initialize();
    }

    ~V8ScriptingEnginePrivate()
    {
    }

    void EncapsulateGlobal(Local<ObjectTemplate>& global)
    {

    }

    QString run(const QString& script)
    {
        QString result;

        // Create a new isolate
        Isolate *isolate = Isolate::New();

        {
            Isolate::Scope scope(isolate);

            // Create a stack-allocated handle scope.
            HandleScope handleScope;

            // Create global object
            Local<ObjectTemplate> global = ObjectTemplate::New();
            EncapsulateGlobal(global);

            // Create a new context.
            Persistent<Context> context = Context::New(0, global);

            // Enter the context for compiling and running the script.
            Context::Scope contextScope(context);

            // Create a string containing the JavaScript source code.
            Local<String> source = String::New(script.toLatin1().data());

            // Catch compilation and evaluation errors
            TryCatch trycatch;

            // Compile the source code.
            Local<Script> _script = Script::Compile(source);
            if (!_script.IsEmpty()) {
                // Run the script to get the result.
                Local<Value> _value = _script->Run();
                if (!_value.IsEmpty())
                    result = QString::fromLatin1(*String::Utf8Value(_value));
                else {
                    String::Utf8Value exception(trycatch.Exception());
                    result = QString(*exception);
                }
            }
            else {
                String::Utf8Value exception(trycatch.Exception());
                result = QString(*exception);
            }
        }

        isolate->Dispose();
        return result;
    }

    QString version() const
    {
        return QString(V8::GetVersion());
    }
};

V8ScriptingEngine::V8ScriptingEngine(QObject *parent)
    : ScriptingEngine(parent), _p(new V8ScriptingEnginePrivate())
{

}

V8ScriptingEngine::~V8ScriptingEngine()
{

}

QString V8ScriptingEngine::version() const
{
    return _p->version();
}

QString V8ScriptingEngine::run(const QString& script)
{
    return _p->run(script);
}

} // namespace engine
} // namespace scripting
} // namespace _9fold
