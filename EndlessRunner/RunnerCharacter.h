// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RunnerCharacter.generated.h"

UCLASS()
class ENDLESSRUNNER_API ARunnerCharacter : public ACharacter
{
	GENERATED_BODY()

		UPROPERTY(VisibleAnywhere)
		class UCameraComponent* SideCamera;

public:
	// Sets default values for this character's properties
	ARunnerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	class UCameraComponent* GetSideCameraComponent() const
	{
		return SideCamera;
	}

	virtual void Landed(const FHitResult& Hit) override;

	void RestartLevel();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(BlueprintReadOnly, Category = Stats)
	int CoinCount = 0;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveRight(float value);

private:
	float zPos;
	FVector tempPos = FVector();

	bool CanMove;

	UPROPERTY()
	bool jumping;

	UPROPERTY()
	int jumpCount;

	void CheckJump();

};
