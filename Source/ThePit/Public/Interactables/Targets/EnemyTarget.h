// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/Target.h"
#include "EnemyTarget.generated.h"

/**
 * 
 */
UCLASS()
class THEPIT_API AEnemyTarget : public ATarget
{
	GENERATED_BODY()
	
public:
	// Functions
	virtual void KnockOver() override;


};
