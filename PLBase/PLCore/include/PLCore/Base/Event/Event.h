/*********************************************************\
 *  File: Event.h                                        *
 *
 *  Copyright (C) 2002-2010 The PixelLight Team (http://www.pixellight.org/)
 *
 *  This file is part of PixelLight.
 *
 *  PixelLight is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  PixelLight is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with PixelLight. If not, see <http://www.gnu.org/licenses/>.
\*********************************************************/


#ifndef __PLCORE_EVENT_H__
#define __PLCORE_EVENT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Base/Event/DynEvent.h"
#include "PLCore/Base/Event/EventHandler.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Generic event class
*
*  @remarks
*    Implementation for up to 16 parameters without a return value
*/
template <typename T0 = NullType, typename T1 = NullType, typename T2 = NullType, typename T3 = NullType, typename T4 = NullType, typename T5 = NullType, typename T6 = NullType, typename T7 = NullType, typename T8 = NullType, typename T9 = NullType, typename T10 = NullType, typename T11 = NullType, typename T12 = NullType, typename T13 = NullType, typename T14 = NullType, typename T15 = NullType>
class Event : public DynEvent {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename Type<T8> ::_Type _T8;
		typedef typename Type<T9> ::_Type _T9;
		typedef typename Type<T10>::_Type _T10;
		typedef typename Type<T11>::_Type _T11;
		typedef typename Type<T12>::_Type _T12;
		typedef typename Type<T13>::_Type _T13;
		typedef typename Type<T14>::_Type _T14;
		typedef typename Type<T15>::_Type _T15;
		typedef EventHandler<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>		TypeHandler;
		typedef Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>	TypeSignature;
		typedef Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>		TypeParams;

		Event() {
		}

		~Event() {
		}

		virtual void Emit(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12, _T13 t13, _T14 t14, _T15 t15) const {
			// [HACK] This is currently necessary to avoid the following strange compiler error:
			//		    undefined reference to `vtable for PLCore::Functor<void, PLCore::Module const*, ...>
			#ifdef GCC
				static Functor<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> DummyFunctor;
			#endif

			// Iterate through all event handlers
			const typename PLGeneral::SimpleList<DynEventHandler*>::ListElement *pElement = m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename PLGeneral::SimpleList<DynEventHandler*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				((TypeHandler*)pElement->Data)->m_cFunctor(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15);

				// Next element, please
				pElement = pNextElement;
			}
		}

		virtual PLGeneral::String GetSignature() const {
			return TypeSignature::GetSignatureID();
		}

		virtual void Emit(const DynParams &cParams) const {
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = (TypeParams&)cParams;

				// Call function
				Emit(cP.Param0,  cP.Param1,  cP.Param2,  cP.Param3,  cP.Param4,
					 cP.Param5,  cP.Param6,  cP.Param7,  cP.Param8,  cP.Param9,
					 cP.Param10, cP.Param11, cP.Param12, cP.Param13, cP.Param14,
					 cP.Param15);
			}
		}
};

/**
*  @brief
*    Generic event class
*
*  @remarks
*    Implementation for 15 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
class Event<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> : public DynEvent {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename Type<T8> ::_Type _T8;
		typedef typename Type<T9> ::_Type _T9;
		typedef typename Type<T10>::_Type _T10;
		typedef typename Type<T11>::_Type _T11;
		typedef typename Type<T12>::_Type _T12;
		typedef typename Type<T13>::_Type _T13;
		typedef typename Type<T14>::_Type _T14;
		typedef EventHandler<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>		TypeHandler;
		typedef Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>	TypeSignature;
		typedef Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>		TypeParams;

		Event() {
		}

		~Event() {
		}

		virtual void Emit(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12, _T13 t13, _T14 t14) const {
			// [HACK] This is currently necessary to avoid the following strange compiler error:
			//		    undefined reference to `vtable for PLCore::Functor<void, PLCore::Module const*, ...>
			#ifdef GCC
				static Functor<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> DummyFunctor;
			#endif

			// Iterate through all event handlers
			const typename PLGeneral::SimpleList<DynEventHandler*>::ListElement *pElement = m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename PLGeneral::SimpleList<DynEventHandler*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				((TypeHandler*)pElement->Data)->m_cFunctor(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14);

				// Next element, please
				pElement = pNextElement;
			}
		}

		virtual PLGeneral::String GetSignature() const {
			return TypeSignature::GetSignatureID();
		}

		virtual void Emit(const DynParams &cParams) const {
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = (TypeParams&)cParams;

				// Call function
				Emit(cP.Param0,  cP.Param1,  cP.Param2,  cP.Param3,  cP.Param4,
					 cP.Param5,  cP.Param6,  cP.Param7,  cP.Param8,  cP.Param9,
					 cP.Param10, cP.Param11, cP.Param12, cP.Param13, cP.Param14);
			}
		}
};

/**
*  @brief
*    Generic event class
*
*  @remarks
*    Implementation for 14 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
class Event<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> : public DynEvent {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename Type<T8> ::_Type _T8;
		typedef typename Type<T9> ::_Type _T9;
		typedef typename Type<T10>::_Type _T10;
		typedef typename Type<T11>::_Type _T11;
		typedef typename Type<T12>::_Type _T12;
		typedef typename Type<T13>::_Type _T13;
		typedef EventHandler<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>	TypeHandler;
		typedef Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>	TypeSignature;
		typedef Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>	TypeParams;

		Event() {
		}

		~Event() {
		}

		virtual void Emit(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12, _T13 t13) const {
			// [HACK] This is currently necessary to avoid the following strange compiler error:
			//		    undefined reference to `vtable for PLCore::Functor<void, PLCore::Module const*, ...>
			#ifdef GCC
				static Functor<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> DummyFunctor;
			#endif

			// Iterate through all event handlers
			const typename PLGeneral::SimpleList<DynEventHandler*>::ListElement *pElement = m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename PLGeneral::SimpleList<DynEventHandler*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				((TypeHandler*)pElement->Data)->m_cFunctor(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13);

				// Next element, please
				pElement = pNextElement;
			}
		}

		virtual PLGeneral::String GetSignature() const {
			return TypeSignature::GetSignatureID();
		}

		virtual void Emit(const DynParams &cParams) const {
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = (TypeParams&)cParams;

				// Call function
				Emit(cP.Param0,  cP.Param1,  cP.Param2,  cP.Param3,  cP.Param4,
					 cP.Param5,  cP.Param6,  cP.Param7,  cP.Param8,  cP.Param9,
					 cP.Param10, cP.Param11, cP.Param12, cP.Param13);
			}
		}
};

/**
*  @brief
*    Generic event class
*
*  @remarks
*    Implementation for 13 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class Event<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> : public DynEvent {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename Type<T8> ::_Type _T8;
		typedef typename Type<T9> ::_Type _T9;
		typedef typename Type<T10>::_Type _T10;
		typedef typename Type<T11>::_Type _T11;
		typedef typename Type<T12>::_Type _T12;
		typedef EventHandler<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>		TypeHandler;
		typedef Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>	TypeSignature;
		typedef Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>		TypeParams;

		Event() {
		}

		~Event() {
		}

		virtual void Emit(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12) const {
			// [HACK] This is currently necessary to avoid the following strange compiler error:
			//		    undefined reference to `vtable for PLCore::Functor<void, PLCore::Module const*, ...>
			#ifdef GCC
				static Functor<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> DummyFunctor;
			#endif

			// Iterate through all event handlers
			const typename PLGeneral::SimpleList<DynEventHandler*>::ListElement *pElement = m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename PLGeneral::SimpleList<DynEventHandler*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				((TypeHandler*)pElement->Data)->m_cFunctor(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12);

				// Next element, please
				pElement = pNextElement;
			}
		}

		virtual PLGeneral::String GetSignature() const {
			return TypeSignature::GetSignatureID();
		}

		virtual void Emit(const DynParams &cParams) const {
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = (TypeParams&)cParams;

				// Call function
				Emit(cP.Param0,  cP.Param1,  cP.Param2,  cP.Param3,  cP.Param4,
					 cP.Param5,  cP.Param6,  cP.Param7,  cP.Param8,  cP.Param9,
					 cP.Param10, cP.Param11, cP.Param12);
			}
		}
};

/**
*  @brief
*    Generic event class
*
*  @remarks
*    Implementation for 12 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class Event<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> : public DynEvent {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename Type<T8> ::_Type _T8;
		typedef typename Type<T9> ::_Type _T9;
		typedef typename Type<T10>::_Type _T10;
		typedef typename Type<T11>::_Type _T11;
		typedef EventHandler<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>		TypeHandler;
		typedef Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>	TypeSignature;
		typedef Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>		TypeParams;

		Event() {
		}

		~Event() {
		}

		virtual void Emit(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11) const {
			// [HACK] This is currently necessary to avoid the following strange compiler error:
			//		    undefined reference to `vtable for PLCore::Functor<void, PLCore::Module const*, ...>
			#ifdef GCC
				static Functor<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> DummyFunctor;
			#endif

			// Iterate through all event handlers
			const typename PLGeneral::SimpleList<DynEventHandler*>::ListElement *pElement = m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename PLGeneral::SimpleList<DynEventHandler*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				((TypeHandler*)pElement->Data)->m_cFunctor(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11);

				// Next element, please
				pElement = pNextElement;
			}
		}

		virtual PLGeneral::String GetSignature() const {
			return TypeSignature::GetSignatureID();
		}

		virtual void Emit(const DynParams &cParams) const {
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = (TypeParams&)cParams;

				// Call function
				Emit(cP.Param0,  cP.Param1,  cP.Param2,  cP.Param3,  cP.Param4,
					 cP.Param5,  cP.Param6,  cP.Param7,  cP.Param8,  cP.Param9,
					 cP.Param10, cP.Param11);
			}
		}
};

/**
*  @brief
*    Generic event class
*
*  @remarks
*    Implementation for 11 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class Event<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> : public DynEvent {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename Type<T8> ::_Type _T8;
		typedef typename Type<T9> ::_Type _T9;
		typedef typename Type<T10>::_Type _T10;
		typedef EventHandler<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>		TypeHandler;
		typedef Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>	TypeSignature;
		typedef Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>		TypeParams;

		Event() {
		}

		~Event() {
		}

		virtual void Emit(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10) const {
			// [HACK] This is currently necessary to avoid the following strange compiler error:
			//		    undefined reference to `vtable for PLCore::Functor<void, PLCore::Module const*, ...>
			#ifdef GCC
				static Functor<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> DummyFunctor;
			#endif

			// Iterate through all event handlers
			const typename PLGeneral::SimpleList<DynEventHandler*>::ListElement *pElement = m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename PLGeneral::SimpleList<DynEventHandler*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				((TypeHandler*)pElement->Data)->m_cFunctor(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10);

				// Next element, please
				pElement = pNextElement;
			}
		}

		virtual PLGeneral::String GetSignature() const {
			return TypeSignature::GetSignatureID();
		}

		virtual void Emit(const DynParams &cParams) const {
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = (TypeParams&)cParams;

				// Call function
				Emit(cP.Param0,  cP.Param1,  cP.Param2,  cP.Param3,  cP.Param4,
					 cP.Param5,  cP.Param6,  cP.Param7,  cP.Param8,  cP.Param9,
					 cP.Param10);
			}
		}
};

/**
*  @brief
*    Generic event class
*
*  @remarks
*    Implementation for 10 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class Event<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> : public DynEvent {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename Type<T8> ::_Type _T8;
		typedef typename Type<T9> ::_Type _T9;
		typedef EventHandler<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>	TypeHandler;
		typedef Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>	TypeSignature;
		typedef Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>	TypeParams;

		Event() {
		}

		~Event() {
		}

		virtual void Emit(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9) const {
			// [HACK] This is currently necessary to avoid the following strange compiler error:
			//		    undefined reference to `vtable for PLCore::Functor<void, PLCore::Module const*, ...>
			#ifdef GCC
				static Functor<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> DummyFunctor;
			#endif

			// Iterate through all event handlers
			const typename PLGeneral::SimpleList<DynEventHandler*>::ListElement *pElement = m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename PLGeneral::SimpleList<DynEventHandler*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				((TypeHandler*)pElement->Data)->m_cFunctor(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9);

				// Next element, please
				pElement = pNextElement;
			}
		}

		virtual PLGeneral::String GetSignature() const {
			return TypeSignature::GetSignatureID();
		}

		virtual void Emit(const DynParams &cParams) const {
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = (TypeParams&)cParams;

				// Call function
				Emit(cP.Param0,  cP.Param1,  cP.Param2,  cP.Param3,  cP.Param4,
					 cP.Param5,  cP.Param6,  cP.Param7,  cP.Param8,  cP.Param9);
			}
		}
};

/**
*  @brief
*    Generic event class
*
*  @remarks
*    Implementation for 9 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class Event<T0, T1, T2, T3, T4, T5, T6, T7, T8> : public DynEvent {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename Type<T8> ::_Type _T8;
		typedef EventHandler<T0, T1, T2, T3, T4, T5, T6, T7, T8>	TypeHandler;
		typedef Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8>	TypeSignature;
		typedef Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8>	TypeParams;

		Event() {
		}

		~Event() {
		}

		virtual void Emit(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8) const {
			// [HACK] This is currently necessary to avoid the following strange compiler error:
			//		    undefined reference to `vtable for PLCore::Functor<void, PLCore::Module const*, ...>
			#ifdef GCC
				static Functor<void, T0, T1, T2, T3, T4, T5, T6, T7, T8> DummyFunctor;
			#endif

			// Iterate through all event handlers
			const typename PLGeneral::SimpleList<DynEventHandler*>::ListElement *pElement = m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename PLGeneral::SimpleList<DynEventHandler*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				((TypeHandler*)pElement->Data)->m_cFunctor(t0, t1, t2, t3, t4, t5, t6, t7, t8);

				// Next element, please
				pElement = pNextElement;
			}
		}

		virtual PLGeneral::String GetSignature() const {
			return TypeSignature::GetSignatureID();
		}

		virtual void Emit(const DynParams &cParams) const {
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = (TypeParams&)cParams;

				// Call function
				Emit(cP.Param0,  cP.Param1,  cP.Param2,  cP.Param3,  cP.Param4,
					 cP.Param5,  cP.Param6,  cP.Param7,  cP.Param8);
			}
		}
};

/**
*  @brief
*    Generic event class
*
*  @remarks
*    Implementation for 8 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class Event<T0, T1, T2, T3, T4, T5, T6, T7> : public DynEvent {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef EventHandler<T0, T1, T2, T3, T4, T5, T6, T7>	TypeHandler;
		typedef Signature<void, T0, T1, T2, T3, T4, T5, T6, T7>	TypeSignature;
		typedef Params<void, T0, T1, T2, T3, T4, T5, T6, T7>	TypeParams;

		Event() {
		}

		~Event() {
		}

		virtual void Emit(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7) const {
			// [HACK] This is currently necessary to avoid the following strange compiler error:
			//		    undefined reference to `vtable for PLCore::Functor<void, PLCore::Module const*, ...>
			#ifdef GCC
				static Functor<void, T0, T1, T2, T3, T4, T5, T6, T7> DummyFunctor;
			#endif

			// Iterate through all event handlers
			const typename PLGeneral::SimpleList<DynEventHandler*>::ListElement *pElement = m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename PLGeneral::SimpleList<DynEventHandler*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				((TypeHandler*)pElement->Data)->m_cFunctor(t0, t1, t2, t3, t4, t5, t6, t7);

				// Next element, please
				pElement = pNextElement;
			}
		}

		virtual PLGeneral::String GetSignature() const {
			return TypeSignature::GetSignatureID();
		}

		virtual void Emit(const DynParams &cParams) const {
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = (TypeParams&)cParams;

				// Call function
				Emit(cP.Param0,  cP.Param1,  cP.Param2,  cP.Param3,  cP.Param4,
					 cP.Param5,  cP.Param6,  cP.Param7);
			}
		}
};

/**
*  @brief
*    Generic event class
*
*  @remarks
*    Implementation for 7 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class Event<T0, T1, T2, T3, T4, T5, T6> : public DynEvent {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef EventHandler<T0, T1, T2, T3, T4, T5, T6>	TypeHandler;
		typedef Signature<void, T0, T1, T2, T3, T4, T5, T6>	TypeSignature;
		typedef Params<void, T0, T1, T2, T3, T4, T5, T6>	TypeParams;

		Event() {
		}

		~Event() {
		}

		virtual void Emit(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6) const {
			// [HACK] This is currently necessary to avoid the following strange compiler error:
			//		    undefined reference to `vtable for PLCore::Functor<void, PLCore::Module const*, ...>
			#ifdef GCC
				static Functor<void, T0, T1, T2, T3, T4, T5, T6> DummyFunctor;
			#endif

			// Iterate through all event handlers
			const typename PLGeneral::SimpleList<DynEventHandler*>::ListElement *pElement = m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename PLGeneral::SimpleList<DynEventHandler*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				((TypeHandler*)pElement->Data)->m_cFunctor(t0, t1, t2, t3, t4, t5, t6);

				// Next element, please
				pElement = pNextElement;
			}
		}

		virtual PLGeneral::String GetSignature() const {
			return TypeSignature::GetSignatureID();
		}

		virtual void Emit(const DynParams &cParams) const {
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = (TypeParams&)cParams;

				// Call function
				Emit(cP.Param0,  cP.Param1,  cP.Param2,  cP.Param3,  cP.Param4,
					 cP.Param5,  cP.Param6);
			}
		}
};

/**
*  @brief
*    Generic event class
*
*  @remarks
*    Implementation for 6 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
class Event<T0, T1, T2, T3, T4, T5> : public DynEvent {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef EventHandler<T0, T1, T2, T3, T4, T5>	TypeHandler;
		typedef Signature<void, T0, T1, T2, T3, T4, T5>	TypeSignature;
		typedef Params<void, T0, T1, T2, T3, T4, T5>	TypeParams;

		Event() {
		}

		~Event() {
		}

		virtual void Emit(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5) const {
			// [HACK] This is currently necessary to avoid the following strange compiler error:
			//		    undefined reference to `vtable for PLCore::Functor<void, PLCore::Module const*, ...>
			#ifdef GCC
				static Functor<void, T0, T1, T2, T3, T4, T5> DummyFunctor;
			#endif

			// Iterate through all event handlers
			const typename PLGeneral::SimpleList<DynEventHandler*>::ListElement *pElement = m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename PLGeneral::SimpleList<DynEventHandler*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				((TypeHandler*)pElement->Data)->m_cFunctor(t0, t1, t2, t3, t4, t5);

				// Next element, please
				pElement = pNextElement;
			}
		}

		virtual PLGeneral::String GetSignature() const {
			return TypeSignature::GetSignatureID();
		}

		virtual void Emit(const DynParams &cParams) const {
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = (TypeParams&)cParams;

				// Call function
				Emit(cP.Param0,  cP.Param1,  cP.Param2,  cP.Param3,  cP.Param4,
					 cP.Param5);
			}
		}
};

/**
*  @brief
*    Generic event class
*
*  @remarks
*    Implementation for 5 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4>
class Event<T0, T1, T2, T3, T4> : public DynEvent {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef EventHandler<T0, T1, T2, T3, T4>	TypeHandler;
		typedef Signature<void, T0, T1, T2, T3, T4>	TypeSignature;
		typedef Params<void, T0, T1, T2, T3, T4>	TypeParams;

		Event() {
		}

		~Event() {
		}

		virtual void Emit(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4) const {
			// [HACK] This is currently necessary to avoid the following strange compiler error:
			//		    undefined reference to `vtable for PLCore::Functor<void, PLCore::Module const*, ...>
			#ifdef GCC
				static Functor<void, T0, T1, T2, T3, T4> DummyFunctor;
			#endif

			// Iterate through all event handlers
			const typename PLGeneral::SimpleList<DynEventHandler*>::ListElement *pElement = m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename PLGeneral::SimpleList<DynEventHandler*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				((TypeHandler*)pElement->Data)->m_cFunctor(t0, t1, t2, t3, t4);

				// Next element, please
				pElement = pNextElement;
			}
		}

		virtual PLGeneral::String GetSignature() const {
			return TypeSignature::GetSignatureID();
		}

		virtual void Emit(const DynParams &cParams) const {
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = (TypeParams&)cParams;

				// Call function
				Emit(cP.Param0,  cP.Param1,  cP.Param2,  cP.Param3,  cP.Param4);
			}
		}
};

/**
*  @brief
*    Generic event class
*
*  @remarks
*    Implementation for 4 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3>
class Event<T0, T1, T2, T3> : public DynEvent {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef EventHandler<T0, T1, T2, T3>	TypeHandler;
		typedef Signature<void, T0, T1, T2, T3>	TypeSignature;
		typedef Params<void, T0, T1, T2, T3>	TypeParams;

		Event() {
		}

		~Event() {
		}

		virtual void Emit(_T0 t0, _T1 t1, _T2 t2, _T3 t3) const {
			// [HACK] This is currently necessary to avoid the following strange compiler error:
			//		    undefined reference to `vtable for PLCore::Functor<void, PLCore::Module const*, ...>
			#ifdef GCC
				static Functor<void, T0, T1, T2, T3> DummyFunctor;
			#endif

			// Iterate through all event handlers
			const typename PLGeneral::SimpleList<DynEventHandler*>::ListElement *pElement = m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename PLGeneral::SimpleList<DynEventHandler*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				((TypeHandler*)pElement->Data)->m_cFunctor(t0, t1, t2, t3);

				// Next element, please
				pElement = pNextElement;
			}
		}

		virtual PLGeneral::String GetSignature() const {
			return TypeSignature::GetSignatureID();
		}

		virtual void Emit(const DynParams &cParams) const {
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = (TypeParams&)cParams;

				// Call function
				Emit(cP.Param0,  cP.Param1,  cP.Param2,  cP.Param3);
			}
		}
};

/**
*  @brief
*    Generic event class
*
*  @remarks
*    Implementation for 3 parameters without a return value
*/
template <typename T0, typename T1, typename T2>
class Event<T0, T1, T2> : public DynEvent {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef EventHandler<T0, T1, T2>	TypeHandler;
		typedef Signature<void, T0, T1, T2>	TypeSignature;
		typedef Params<void, T0, T1, T2>	TypeParams;

		Event() {
		}

		~Event() {
		}

		virtual void Emit(_T0 t0, _T1 t1, _T2 t2) const {
			// [HACK] This is currently necessary to avoid the following strange compiler error:
			//		    undefined reference to `vtable for PLCore::Functor<void, PLCore::Module const*, ...>
			#ifdef GCC
				static Functor<void, T0, T1, T2> DummyFunctor;
			#endif

			// Iterate through all event handlers
			const typename PLGeneral::SimpleList<DynEventHandler*>::ListElement *pElement = m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename PLGeneral::SimpleList<DynEventHandler*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				((TypeHandler*)pElement->Data)->m_cFunctor(t0, t1, t2);

				// Next element, please
				pElement = pNextElement;
			}
		}

		virtual PLGeneral::String GetSignature() const {
			return TypeSignature::GetSignatureID();
		}

		virtual void Emit(const DynParams &cParams) const {
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = (TypeParams&)cParams;

				// Call function
				Emit(cP.Param0,  cP.Param1,  cP.Param2);
			}
		}
};

/**
*  @brief
*    Generic event class
*
*  @remarks
*    Implementation for 2 parameters without a return value
*/
template <typename T0, typename T1>
class Event<T0, T1> : public DynEvent {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef EventHandler<T0, T1>		TypeHandler;
		typedef Signature<void, T0, T1>		TypeSignature;
		typedef Params<void, T0, T1>		TypeParams;

		Event() {
		}

		~Event() {
		}

		virtual void Emit(_T0 t0, _T1 t1) const {
			// [HACK] This is currently necessary to avoid the following strange compiler error:
			//		    undefined reference to `vtable for PLCore::Functor<void, PLCore::Module const*, ...>
			#ifdef GCC
				static Functor<void, T0, T1> DummyFunctor;
			#endif

			// Iterate through all event handlers
			const PLGeneral::SimpleList<DynEventHandler*>::ListElement *pElement = m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const PLGeneral::SimpleList<DynEventHandler*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				((TypeHandler*)pElement->Data)->m_cFunctor(t0, t1);

				// Next element, please
				pElement = pNextElement;
			}
		}

		virtual PLGeneral::String GetSignature() const {
			return TypeSignature::GetSignatureID();
		}

		virtual void Emit(const DynParams &cParams) const {
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = (TypeParams&)cParams;

				// Call function
				Emit(cP.Param0,  cP.Param1);
			}
		}
};

/**
*  @brief
*    Generic event class
*
*  @remarks
*    Implementation for 1 parameter without a return value
*/
template <typename T0>
class Event<T0> : public DynEvent {
	public:
		typedef typename Type<T0>::_Type _T0;

	public:
		typedef EventHandler<T0>	TypeHandler;
		typedef Signature<void, T0>	TypeSignature;
		typedef Params<void, T0>	TypeParams;

		Event() {
		}

		~Event() {
		}

		virtual void Emit(_T0 t0) const {
			// [HACK] This is currently necessary to avoid the following strange compiler error:
			//		    undefined reference to `vtable for PLCore::Functor<void, PLCore::Module const*, ...>
			#ifdef GCC
				static Functor<void, T0> DummyFunctor;
			#endif

			// Iterate through all event handlers
			const PLGeneral::SimpleList<DynEventHandler*>::ListElement *pElement = m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const PLGeneral::SimpleList<DynEventHandler*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				((TypeHandler*)pElement->Data)->m_cFunctor(t0);

				// Next element, please
				pElement = pNextElement;
			}
		}

		virtual PLGeneral::String GetSignature() const {
			return TypeSignature::GetSignatureID();
		}

		virtual void Emit(const DynParams &cParams) const {
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = (TypeParams&)cParams;

				// Call function
				Emit(cP.Param0);
			}
		}
};

/**
*  @brief
*    Generic event class
*
*  @remarks
*    Implementation for 0 parameters without a return value
*/
template <>
class Event<> : public DynEvent {
	public:
		typedef EventHandler<>	TypeHandler;
		typedef Signature<void>	TypeSignature;
		typedef Params<void>	TypeParams;

		Event() {
		}

		~Event() {
		}

		virtual void Emit() const {
			// [HACK] This is currently necessary to avoid the following strange compiler error:
			//		    undefined reference to `vtable for PLCore::Functor<void, PLCore::Module const*, ...>
			#ifdef GCC
				static Functor<void> DummyFunctor;
			#endif

			// Iterate through all event handlers
			const PLGeneral::SimpleList<DynEventHandler*>::ListElement *pElement = m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const PLGeneral::SimpleList<DynEventHandler*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				((TypeHandler*)pElement->Data)->m_cFunctor();

				// Next element, please
				pElement = pNextElement;
			}
		}

		virtual PLGeneral::String GetSignature() const {
			return TypeSignature::GetSignatureID();
		}

		virtual void Emit(const DynParams &cParams) const {
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Call function
				Emit();
			}
		}
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_EVENT_H__
