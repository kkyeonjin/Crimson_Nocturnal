#pragma once

UENUM(BlueprintType)
enum class ESkillConfirmType : uint8
{
    UltimateSkill, //Section 0 in SkillTreeUIWidget
    NormalSkill, //Section 1, SkillCount 0-3 in SkillTreeUIWidget
    SkillTimeUp, //Section 1, SkillCount4 in SkillTreeUIWidget
    KeyBindingX, //Section 2 in SkillTreeUIWidget
    KeyBindingY //Section 2 in SkillTreeUIWidget

};

// Combat State Sections for SkillTree
enum class ESkillTreeSection : uint8
{
    UltimateSkill = 0,      // Section 0
    NormalSkill = 1,        // Section 1
    SkillASetting = 2       // Section 2
};
