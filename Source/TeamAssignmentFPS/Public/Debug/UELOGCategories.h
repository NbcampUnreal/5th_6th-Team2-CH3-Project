// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//여기에 자신이 쓰는 로그 카테고리 만들어서 본인 작업 장소에서 헤더추가해서 작업하기/ 10/21/2025

// 현 API + Log Declare + 이름

 TEAMASSIGNMENTFPS_API DECLARE_LOG_CATEGORY_EXTERN(UI_Log, Log, All);// UI 관련 디버깅 카테고리

 TEAMASSIGNMENTFPS_API DECLARE_LOG_CATEGORY_EXTERN(IMC_Log, Log, All);// IMC management 디버깅

 TEAMASSIGNMENTFPS_API DECLARE_LOG_CATEGORY_EXTERN(Controller_Log, Log, All);// Controller

 TEAMASSIGNMENTFPS_API DECLARE_LOG_CATEGORY_EXTERN(Camera_Log, Log, All);// Camera

 TEAMASSIGNMENTFPS_API DECLARE_LOG_CATEGORY_EXTERN(Movement_Log, Log, All);// Character Movement

 TEAMASSIGNMENTFPS_API DECLARE_LOG_CATEGORY_EXTERN(Character_Log, Log, All);// player Character

 TEAMASSIGNMENTFPS_API DECLARE_LOG_CATEGORY_EXTERN(TimeManager_Log, Log, All);// time flow manipulation

 TEAMASSIGNMENTFPS_API DECLARE_LOG_CATEGORY_EXTERN(LockonTarget_Log, Log, All);// LockonTarget

 TEAMASSIGNMENTFPS_API DECLARE_LOG_CATEGORY_EXTERN(Enemy_Log, Warning, All); // Enemy

 TEAMASSIGNMENTFPS_API DECLARE_LOG_CATEGORY_EXTERN(GameState_Log, Log, All);

TEAMASSIGNMENTFPS_API DECLARE_LOG_CATEGORY_EXTERN(Equipment_Manager_Log, Log, All);

TEAMASSIGNMENTFPS_API DECLARE_LOG_CATEGORY_EXTERN(Weapon_Log, Log, All);

TEAMASSIGNMENTFPS_API DECLARE_LOG_CATEGORY_EXTERN(Item_Log, Log, All);
