//
//  Generated file. Do not edit.
//

#include "generated_plugin_registrant.h"

#include <win_messages_plugin.h>
#include <window_size_plugin.h>

void RegisterPlugins(flutter::PluginRegistry* registry) {
  WinMessagesPluginRegisterWithRegistrar(
      registry->GetRegistrarForPlugin("WinMessagesPlugin"));
  WindowSizePluginRegisterWithRegistrar(
      registry->GetRegistrarForPlugin("WindowSizePlugin"));
}
