/*
 * Copyright (C) 2017-2018 Tauri JayD <https://www.>
 * Copyright (c) 2016 OmniGUI Platform
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "AttachableMember.h"

XamlCPP::Core::Proto::XamlType * XamlCPP::Core::ReflEx::AttachableMember::LookupType()
{
    if (Getter)
        return TypeRepository->GetByType(Getter->GetMethodInfo().m_returnType);

    throw System::AggregateException("Cannot find a property named \"{Name}\" in the type {underlyingType}");
    return nullptr;
}

System::Reflection::meta::Field const * XamlCPP::Core::ReflEx::AttachableMember::RuntimeProperty()
{
    return &DeclaringType->UnderlyingType()->GetProperty(Name);
}

XamlCPP::Core::ReflEx::AttachableMember::AttachableMember(std::string const & name, const System::Reflection::IMethod* getter, const System::Reflection::IMethod* setter, Interfaces::ITypeRepository * typeRepository, Interfaces::ITypeFeatureProvider * featureProvider)
    : MutableMember(name, typeRepository->GetByType(&getter->DeclaringType()), typeRepository, featureProvider)
{
    Getter = getter;
    Setter = setter;
    XamlType = LookupType();
}

bool XamlCPP::Core::ReflEx::AttachableMember::IsDirective() const
{
    return false;
}

bool XamlCPP::Core::ReflEx::AttachableMember::IsAttachable() const
{
    return true;
}
