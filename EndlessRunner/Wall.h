// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spike.h"
#include "Wall.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSRUNNER_API AWall : public ASpike
{
	GENERATED_BODY()


public:

	AWall();

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

	
};
