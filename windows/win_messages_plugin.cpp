#include "win_messages_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <map>
#include <memory>
#include <sstream>

namespace {

class WinMessagesPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

  WinMessagesPlugin(flutter::PluginRegistrarWindows *registrar);

  virtual ~WinMessagesPlugin();

  UINT32 toUINT32(std::vector<uint8_t> val);
  UINT64 toUINT64(std::vector<uint8_t> val);

 private:
  // Called when a method is called on this plugin's channel from Dart.
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

  // The registrar for this plugin, for accessing the window.
  flutter::PluginRegistrarWindows *registrar_;
};

// static
void WinMessagesPlugin::RegisterWithRegistrar(
    flutter::PluginRegistrarWindows *registrar) {
  auto channel =
      std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
          registrar->messenger(), "win_messages",
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

WinMessagesPlugin::~WinMessagesPlugin() {}

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
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  // Replace "getPlatformVersion" check with your plugin's method.
  // See:
  // https://github.com/flutter/engine/tree/master/shell/platform/common/cpp/client_wrapper/include/flutter
  // and
  // https://github.com/flutter/engine/tree/master/shell/platform/glfw/client_wrapper/include/flutter
  // for the relevant Flutter APIs.
  if (method_call.method_name().compare("getPlatformVersion") == 0) {
    std::ostringstream version_stream;
    version_stream << "Windows ";
    if (IsWindows10OrGreater()) {
      version_stream << "10+";
    } else if (IsWindows8OrGreater()) {
      version_stream << "8";
    } else if (IsWindows7OrGreater()) {
      version_stream << "7";
    }
    flutter::EncodableValue response(version_stream.str());
    result->Success(&response);
  } else if (method_call.method_name().compare("SendMessage") == 0)
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
        lParam = static_cast<LPARAM>(toUINT64(args_list[2].ByteListValue()));
    }
    SendMessage(GetParent(this->registrar_->GetView()->GetNativeWindow()), msg, wParam, lParam);
  } else {
    result->NotImplemented();
  }
}

}  // namespace

void WinMessagesPluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  // The plugin registrar wrappers owns the plugins, registered callbacks, etc.,
  // so must remain valid for the life of the application.
  static auto *plugin_registrars =
      new std::map<FlutterDesktopPluginRegistrarRef,
                   std::unique_ptr<flutter::PluginRegistrarWindows>>;
  auto insert_result = plugin_registrars->emplace(
      registrar, std::make_unique<flutter::PluginRegistrarWindows>(registrar));

  WinMessagesPlugin::RegisterWithRegistrar(insert_result.first->second.get());
}
