/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
/*
 * \file src/runtime/object_internal.h
 * \brief Expose a few functions for CFFI purposes.
 *        This file is not intended to be used
 */
#ifndef TVM_RUNTIME_OBJECT_INTERNAL_H_
#define TVM_RUNTIME_OBJECT_INTERNAL_H_

#include <tvm/runtime/module.h>
#include <tvm/runtime/object.h>

#include <string>
#include <utility>

namespace tvm {
namespace runtime {

/*!
 * \brief Internal object namespace to expose
 *        certain util functions for FFI.
 */
class ObjectInternal {
 public:
  /*!
   * \brief Retain an object handle.
   */
  static void ObjectRetain(TVMObjectHandle obj) {
    if (obj != nullptr) {
      // static_cast<Object*>(obj)->IncRef();
      tvm::ffi::details::ObjectUnsafe::IncRefObjectHandle(obj);
    }
  }

  /*!
   * \brief Free an object handle.
   */
  static void ObjectFree(TVMObjectHandle obj) {
    if (obj != nullptr) {
      // static_cast<Object*>(obj)->DecRef();
      tvm::ffi::details::ObjectUnsafe::DecRefObjectHandle(obj);
    }
  }
  /*!
   * \brief Check of obj derives from the type indicated by type index.
   * \param obj The original object.
   * \param type_index The type index of interest.
   * \return The derivation checking result.
   */
  // static bool DerivedFrom(const Object* obj, uint32_t type_index) {
  //  return obj->DerivedFrom(type_index);
  // }
  /*!
   * \brief Expose TypeKey2Index
   * \param type_key The original type key.
   * \return the corresponding index.
   */
  static uint32_t ObjectTypeKey2Index(const std::string& type_key) {
    int32_t type_index;
    TVMFFIByteArray type_key_arr{type_key.data(), type_key.length()};
    TVM_FFI_CHECK_SAFE_CALL(TVMFFITypeKeyToIndex(&type_key_arr, &type_index));
    return static_cast<uint32_t>(type_index);
  }
  /*!
   * \brief Convert ModuleHandle to module node pointer.
   * \param handle The module handle.
   * \return the corresponding module node pointer.
   */
  static ModuleNode* GetModuleNode(TVMModuleHandle handle) {
    // NOTE: we will need to convert to Object
    // then to ModuleNode in order to get the correct
    // address translation
    return static_cast<ModuleNode*>(static_cast<Object*>(handle));
  }
};

}  // namespace runtime
}  // namespace tvm
#endif  // TVM_RUNTIME_OBJECT_INTERNAL_H_
