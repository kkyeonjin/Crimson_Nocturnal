// Fill out your copyright notice in the Description page of Project Settings.

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

