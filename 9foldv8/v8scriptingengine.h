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

#ifndef V8SCRIPTINGENGINE_H
#define V8SCRIPTINGENGINE_H

#include <scriptingengine.h>

#include <QObject>
#include <QScopedPointer>

namespace _9fold
{
namespace scripting
{
namespace engine
{

class V8ScriptingEngine : public ScriptingEngine
{
    Q_OBJECT
public:
    V8ScriptingEngine(QObject *parent = 0);
    virtual ~V8ScriptingEngine();

    virtual QString version() const;

    virtual QString run(const QString& script);

private:
    class V8ScriptingEnginePrivate;
    QScopedPointer<V8ScriptingEnginePrivate> _p;
};

} // namespace engine
} // namespace scripting
} // namespace _9fold

#endif // V8SCRIPTINGENGINE_H
