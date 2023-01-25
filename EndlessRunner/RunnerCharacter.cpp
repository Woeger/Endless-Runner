// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Spike.h"
#include "Wall.h"
#include "Coin.h"
#include "Engine.h"

// Sets default values
ARunnerCharacter::ARunnerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	//Set capsule size + collision channel
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);

	//Disable controller rotation
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	//Create Camera object + disable rotation
	SideCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Side Camera"));
	SideCamera->bUsePawnControlRotation = false;

	//Setup character movement variables
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.8f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	//Set default position
	tempPos = GetActorLocation();
	zPos = tempPos.Z = 300.f;

	jumping = false;
	jumpCount = 0;


}

// Called when the game starts or when spawned
void ARunnerCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ARunnerCharacter::OnOverlapBegin);

	CanMove = true;
	
}

void ARunnerCharacter::MoveRight(float value)
{
	if (CanMove)
	{
		AddMovementInput(FVector(0.f, 1.f, 0.f), value);
	}
}

void ARunnerCharacter::CheckJump()
{
	if (jumping)
	{
		jumping = false;
	}

	else
	{
		jumping = true;
		jumpCount++;
		
		if (jumpCount == 2)
		{
			LaunchCharacter(FVector(0, 0, 500), false, true);
		}
	}
}

// Called every frame
void ARunnerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (jumping)
	{
		Jump();
	}

	//Update camera every tick
	tempPos = GetActorLocation();
	tempPos.X -= 850.f;
	tempPos.Z = zPos;
	SideCamera->SetWorldLocation(tempPos);

}

// Called to bind functionality to input
void ARunnerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Bind actions from player class
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ARunnerCharacter::CheckJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ARunnerCharacter::CheckJump);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARunnerCharacter::MoveRight);

}

void ARunnerCharacter::RestartLevel()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()));
}

void ARunnerCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor != nullptr)
	{
		ASpike* Wall = Cast<AWall>(OtherActor);
		ASpike* Spike = Cast<ASpike>(OtherActor);
		ACoin* Coin = Cast<ACoin>(OtherActor);

		if (Wall || Spike)
		{
			GetMesh()->Deactivate();
			GetMesh()->SetVisibility(false);


			CanMove = false;

			FTimerHandle UnusedHandle;
			GetWorldTimerManager().SetTimer(UnusedHandle, this, &ARunnerCharacter::RestartLevel, 2.f, false);
		}

		else if (Coin)
		{
			CoinCount++;

			OtherActor->Destroy();
			
		}
	}
}

void ARunnerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	jumpCount = 0;

}

