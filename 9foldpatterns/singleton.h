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

#ifndef SINGLETON_H
#define SINGLETON_H

namespace _9fold
{
namespace patterns
{

template<class T>
class Singleton
{
public:
    static T& instance()
    {
        // In C++11 this is thread and memory safe so there's no
        // need to use double-check locking or similar technique
        static T t;
        return t;
    }

private:
    // Non-copyable
    Singleton() = delete;
    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton singleton) = delete;
};

} // namspace patterns
} // namespace _9fold

#endif // SINGLETON_H

