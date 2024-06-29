/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/extensions/api/pkcs11_api.h"

#include <memory>
#include <string>

#include "base/logging.h"
#include "base/json/json_writer.h"
#include "base/values.h"
#include "brave/common/extensions/api/pkcs11.h"

namespace extensions {
namespace api {

ExtensionFunction::ResponseAction Pkcs11InstallModuleFunction::Run() {
  absl::optional<pkcs11::InstallModule::Params> params =
      pkcs11::InstallModule::Params::Create(args());
  EXTENSION_FUNCTION_VALIDATE(params);

  LOG(INFO) << "Setting path: " << params->path << " for PKCS11 module.";

  return RespondNow(NoArguments());
}

ExtensionFunction::ResponseAction Pkcs11SetPinFunction::Run() {
  absl::optional<pkcs11::SetPin::Params> params =
      pkcs11::SetPin::Params::Create(args());
  EXTENSION_FUNCTION_VALIDATE(params);

  LOG(INFO) << "Setting pin: " << params->pin << " for PKCS11 login.";

  return RespondNow(NoArguments());
}

ExtensionFunction::ResponseAction Pkcs11GetSignatureFunction::Run() {
  const std::string signature = "sample_signature";

  return RespondNow(WithArguments(signature));
}

}  // namespace api
}  // namespace extensions
