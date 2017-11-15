// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombiETS.h"
#include "AIZombie.h"
#include "AIZombieController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"

#define COLLISION_SWORD ECollisionChannel::ECC_GameTraceChannel1

float AAIZombie::speedMultiplier = 0.0f;

// Sets default values
AAIZombie::AAIZombie(const FObjectInitializer& objectInitializer)
	:Super(objectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Init senses
	PawnSensingComp = CreateEditorOnlyDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->SetPeripheralVisionAngle(90.f);

	//Init overlapColision
	overlapColision = CreateDefaultSubobject<USphereComponent>(TEXT("RootCollision"));
	overlapColision->SetSphereRadius(80.0f);
	//overlapColision->SetHiddenInGame(false);
	overlapColision->OnComponentBeginOverlap.AddDynamic(this, &AAIZombie::OnZombieOverlap);
	overlapColision->SetCollisionObjectType(COLLISION_SWORD);
	overlapColision->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void AAIZombie::BeginPlay()
{
	Super::BeginPlay();
	baseSpeed = GetCharacterMovement()->MaxWalkSpeed;
	if (PawnSensingComp)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &AAIZombie::OnPlayerCaught);
	}
	
}

// Called every frame
void AAIZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (speedMultiplier == 0.0f)
		GetCharacterMovement()->MaxWalkSpeed = baseSpeed;
	else
		GetCharacterMovement()->MaxWalkSpeed = baseSpeed * speedMultiplier;
}

// Called to bind functionality to input
void AAIZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
		
}

void AAIZombie::OnPlayerCaught(APawn * Pawn)
{
	//Get a reference to the player controller
	AAIZombieController* AIController = Cast<AAIZombieController>(GetController());

	if (AIController)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Zombie: I can see you !!!"));
		AIController->SetPlayerCaught(Pawn);
	}
}

void AAIZombie::OnZombieOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Zombie: Ouch, you hit me."));
}

float AAIZombie::GetSpeedMultiplier()
{
	return speedMultiplier;
}

float AAIZombie::SetSpeedMultiplier(float speed)
{
	return speedMultiplier = speed;
}

