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
        : _isolate(0)
    {
        V8::Initialize();

        _isolate = Isolate::New();
        Q_ASSERT(_isolate);

        HandleScope _handleScope;

        _global = ObjectTemplate::New();
        Q_ASSERT(!_global.IsEmpty());

        _context = Context::New(0, _global);
        Q_ASSERT(!_context.IsEmpty());

        EncapsulateGlobal();
    }

    ~V8ScriptingEnginePrivate()
    {
        _isolate->Dispose();
        V8::Dispose();
    }

    void EncapsulateGlobal()
    {

    }

    QString run(const QString& script)
    {
        Local<String> _source = String::New(script.toLatin1().data());
        Local<Script> _script = Script::Compile(_source);
        Local<Value> _value = _script->Run();
        return QString(QString::fromLatin1(*String::Utf8Value(_value)));
    }

private:
    Isolate *_isolate;
    Handle<ObjectTemplate> _global;
    Persistent<Context> _context;
};

V8ScriptingEngine::V8ScriptingEngine(QObject *parent)
    : ScriptingEngine(parent), _p(new V8ScriptingEnginePrivate())
{

}

V8ScriptingEngine::~V8ScriptingEngine()
{

}

QString V8ScriptingEngine::run(const QString& script)
{
    return _p->run(script);
}

} // namespace engine
} // namespace scripting
} // namespace _9fold
