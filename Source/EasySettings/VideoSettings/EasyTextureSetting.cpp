

#include "EasyTextureSetting.h"
#include "GameFramework/GameUserSettings.h"

void UEasyTextureSetting::SetValue_Logic_Implementation(int NewValue)
{
    UGameUserSettings::GetGameUserSettings()->SetTextureQuality(NewValue);
    OnSettingChanged.Broadcast(this);
    OnValueChanged.Broadcast(NewValue);
}

int UEasyTextureSetting::GetValue_Implementation() const
{
    return UGameUserSettings::GetGameUserSettings()->GetTextureQuality();
}

UEasyTextureSetting::UEasyTextureSetting()
{
    SettingName = NSLOCTEXT("EasySettings", "TextureSetting", "Texture Quality");
}
