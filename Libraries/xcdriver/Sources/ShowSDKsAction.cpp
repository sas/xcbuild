/**
 Copyright (c) 2015-present, Facebook, Inc.
 All rights reserved.

 This source code is licensed under the BSD-style license found in the
 LICENSE file in the root directory of this source tree. An additional grant
 of patent rights can be found in the PATENTS file in the same directory.
 */

#include <xcdriver/ShowSDKsAction.h>
#include <xcdriver/Options.h>
#include <xcsdk/Configuration.h>
#include <xcsdk/Environment.h>
#include <xcsdk/SDK/Manager.h>
#include <xcsdk/SDK/Platform.h>
#include <xcsdk/SDK/Target.h>
#include <libutil/Filesystem.h>
#include <libutil/ProcessContext.h>

using xcdriver::ShowSDKsAction;
using xcdriver::Options;
using libutil::Filesystem;
using libutil::ProcessContext;

ShowSDKsAction::
ShowSDKsAction()
{
}

ShowSDKsAction::
~ShowSDKsAction()
{
}

int ShowSDKsAction::
Run(ProcessContext const *processContext, Filesystem const *filesystem, Options const &options)
{
    ext::optional<std::string> developerRoot = xcsdk::Environment::DeveloperRoot(processContext, filesystem);
    if (!developerRoot) {
        fprintf(stderr, "error: unable to find developer dir\n");
        return 1;
    }

    auto configuration = xcsdk::Configuration::Load(filesystem, xcsdk::Configuration::DefaultPaths(processContext));
    auto manager = xcsdk::SDK::Manager::Open(filesystem, *developerRoot, configuration);
    if (manager == nullptr) {
        fprintf(stderr, "error: unable to open developer directory\n");
        return 1;
    }

    for (auto const &platform : manager->platforms()) {
        printf("%s SDKs:\n", platform->description().c_str());
        for (auto const &target : platform->targets()) {
            printf("\t%-32s-sdk %s\n", target->displayName().c_str(), target->canonicalName().c_str());
        }
        printf("\n");
    }

    return 0;
}
