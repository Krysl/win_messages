// Copyright 2019 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include "win_messages_plugin.h"

#include <ShellScalingApi.h>
#include <windows.h>

#include <VersionHelpers.h>
#include <flutter/flutter_view.h>
#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>
#include <memory>
#include <sstream>
#include <codecvt>

namespace
{

using flutter::EncodableList;
using flutter::EncodableMap;
using flutter::EncodableValue;

// See win_mesages_channel.dart for documentation.
const char kChannelName[] = "flutter/win_messages";
const char kSendMessageMethod[] = "SendMessage";

class WinMessagesPlugin : public flutter::Plugin
{
public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

  // Creates a plugin that communicates on the given channel.
  WinMessagesPlugin(flutter::PluginRegistrarWindows *registrar);

  virtual ~WinMessagesPlugin();

  UINT32 toUINT32(std::vector<uint8_t> val);
  UINT64 toUINT64(std::vector<uint8_t> val);

private:
  // Called when a method is called on the plugin channel;
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

  // The registrar for this plugin, for accessing the window.
  flutter::PluginRegistrarWindows *registrar_;
};

// static
void WinMessagesPlugin::RegisterWithRegistrar(
    flutter::PluginRegistrarWindows *registrar)
{
  auto channel =
      std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
          registrar->messenger(), kChannelName,
          &flutter::StandardMethodCodec::GetInstance());

  auto plugin = std::make_unique<WinMessagesPlugin>(registrar);

  channel->SetMethodCallHandler(
      [plugin_pointer = plugin.get()](const auto &call, auto result) {
        plugin_pointer->HandleMethodCall(call, std::move(result));
      });

  registrar->AddPlugin(std::move(plugin));
}

WinMessagesPlugin::WinMessagesPlugin(flutter::PluginRegistrarWindows *registrar)
    : registrar_(registrar) {}

WinMessagesPlugin::~WinMessagesPlugin(){};

UINT32 WinMessagesPlugin::toUINT32(std::vector<uint8_t> val)
{
    UINT ret = 0;
    assert(val.size() == 4);
    for (int i = 0; i < 4; i++)
    {
        ret += val[i] << (i << 3);
    }
    return ret;
}


UINT64 WinMessagesPlugin::toUINT64(std::vector<uint8_t> val)
{
    UINT ret = 0;
    assert(val.size() == 8);
    for (int i = 0; i < 8; i++)
    {
        ret += val[i] << (i << 3);
    }
    return ret;
}

void WinMessagesPlugin::HandleMethodCall(
    const flutter::MethodCall<flutter::EncodableValue> &method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
  if (method_call.method_name().compare(kSendMessageMethod) == 0)
  {
    if (!method_call.arguments() || !method_call.arguments()->IsList() ||
        method_call.arguments()->ListValue().size() != 3)
    {
      result->Error("Bad arguments", "Expected 3-element list");
      return;
    }
    const auto &args_list = method_call.arguments()->ListValue();
    UINT msg = static_cast<UINT>(toUINT32(args_list[0].ByteListValue()));
    WPARAM wParam = NULL;
    if (args_list[1].IsByteList())
    {
        wParam = static_cast<WPARAM>(toUINT64(args_list[1].ByteListValue()));;
    }
    LPARAM lParam = NULL;
    if (args_list[2].IsByteList())
    {
        static_cast<LPARAM>(toUINT64(args_list[2].ByteListValue()));
    }
    SendMessage(GetParent(this->registrar_->GetView()->GetNativeWindow()), msg, wParam, lParam);
  }
  else
  {
    result->NotImplemented();
  }
}

} // namespace

void WinMessagesPluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar)
{
  // The plugin registrar owns the plugin, registered callbacks, etc., so must
  // remain valid for the life of the application.
  static auto *plugin_registrar =
      new flutter::PluginRegistrarWindows(registrar);

  WinMessagesPlugin::RegisterWithRegistrar(plugin_registrar);
}
