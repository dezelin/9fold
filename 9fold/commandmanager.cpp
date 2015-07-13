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

#include "commandmanager.h"

#include <QMutex>
#include <QMutexLocker>
#include <QQueue>
#include <QThread>
#include <QWaitCondition>

namespace _9fold
{
namespace commands
{

class CommandManager::CommandManagerPrivate : public QThread
{
public:
    CommandManagerPrivate(QObject *parent)
        : QThread(parent), _quit(false), _workspace(0)
    {
        start();
    }

    virtual ~CommandManagerPrivate()
    {
        quit();
        wait();
    }

    Workspace* workspace() const
    {
        return _workspace;
    }

    void setWorkspace(Workspace *workspace)
    {
        Q_ASSERT(workspace);
        _workspace = workspace;
    }

    void enqueue(CommandManager::BackgroundCommand *command)
    {
        Q_ASSERT(!_quit);
        QMutexLocker lock(&_mutex);
        _queue.append(command);
        _condition.wakeOne();
    }

protected:
    void quit()
    {
        QMutexLocker lock(&_mutex);
        _quit = true;
        _condition.wakeOne();
    }

    virtual void run();

private:
    bool _quit;
    Workspace *_workspace;
    QMutex _mutex;
    QWaitCondition _condition;
    QQueue<CommandManager::BackgroundCommand*> _queue;
};

void CommandManager::CommandManagerPrivate::run()
{
    BackgroundCommand* command = 0;

    while(!_quit) {
        {
            QMutexLocker lock(&_mutex);
            if (_queue.size() < 1) {
                _condition.wait(&_mutex);
                if (_quit)
                    break;
            }

            command = _queue.dequeue();
        }

        Q_ASSERT(command);
        command->execute();
        delete command;
    }

    // Dequeue remaining commands
    QMutexLocker lock(&_mutex);
    while(_queue.size() > 0) {
        command = _queue.dequeue();
        Q_ASSERT(command);
        delete command;
    }
}

CommandManager::CommandManager(QObject *parent)
    : QObject(parent), _p(new CommandManagerPrivate(this))
{

}

CommandManager::~CommandManager()
{

}

Workspace* CommandManager::workspace() const
{
    return _p->workspace();
}

void CommandManager::setWorkspace(Workspace *workspace)
{
    _p->setWorkspace(workspace);
}

void CommandManager::enqueue(BackgroundCommand *command)
{
    Q_ASSERT(command);
    Q_ASSERT(!dynamic_cast<GuiCommand*>(command));
    if (!command)
        return;

    _p->enqueue(command);
}

void CommandManager::enqueue(GuiCommand *command)
{
    Q_ASSERT(command);
    if (!command)
        return;

    command->execute();
}

} // namespace commands
} // namespace _9fold


