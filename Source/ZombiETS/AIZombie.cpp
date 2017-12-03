// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombiETS.h"
#include "AIZombie.h"
#include "AIZombieController.h"
#include "ZombiETSCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"
#include "ZombiETSGameMode.h"

#define COLLISION_SWORD ECollisionChannel::ECC_GameTraceChannel1
#define COLLISION_PLAYER ECollisionChannel::ECC_GameTraceChannel2

float AAIZombie::speedMultiplier = 1.0f;

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
	overlapSwordColision = CreateDefaultSubobject<USphereComponent>(TEXT("RootSwordCollision"));
	overlapSwordColision->SetSphereRadius(80.0f);
	//overlapSwordColision->SetHiddenInGame(false);
	overlapSwordColision->OnComponentBeginOverlap.AddDynamic(this, &AAIZombie::OnZombieOverlap);
	overlapSwordColision->SetCollisionObjectType(COLLISION_SWORD);
	overlapSwordColision->SetupAttachment(RootComponent);

	//Init overlapPlayerColision
	overlapPlayerColision = CreateDefaultSubobject<USphereComponent>(TEXT("RootPlayerCollision"));
	overlapPlayerColision->SetSphereRadius(80.0f);
	overlapPlayerColision->OnComponentBeginOverlap.AddDynamic(this, &AAIZombie::OnZombieOverlap);
	overlapPlayerColision->SetCollisionObjectType(COLLISION_PLAYER);
	overlapPlayerColision->SetupAttachment(RootComponent);

	// Set a base health amount for the main character
	InitialHealth = 1000.f;
	ZombieHealth = InitialHealth;

	//init animations
	static ConstructorHelpers::FObjectFinder<UAnimSequence> deathAnim(TEXT("AnimSequence'/Game/AI/Zombie/Skins/ZombieYaku/Animations/Yaku_Zombie_Death_Anim_mixamo_com'"));
	deathAnimation = deathAnim.Object;

	static ConstructorHelpers::FObjectFinder<UAnimSequence> hitAnim(TEXT("AnimSequence'/Game/AI/Zombie/Skins/ZombieYaku/Animations/Yaku_Zombie_Reaction_Hit_Anim_mixamo_com'"));
	hitAnimation = hitAnim.Object;

	static ConstructorHelpers::FObjectFinder<UAnimSequence> attAnim(TEXT("AnimSequence'/Game/AI/Zombie/Skins/ZombieYaku/Animations/Yaku_Zombie_Attack_Anim_mixamo_com'"));
	attackAnimation = attAnim.Object;
	
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

void AAIZombie::AlertGameModeOfDeath()
{
	AZombiETSGameMode* gameMode = (AZombiETSGameMode*)GetWorld()->GetAuthGameMode();
	gameMode->ZombieKilled(1);
}

// Called every frame
void AAIZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GetCharacterMovement()->MaxWalkSpeed = baseSpeed * speedMultiplier;
}

// Called to bind functionality to input
void AAIZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
		
}

void AAIZombie::OnPlayerCaught(APawn * Pawn)
{
	if (!isDead) {
		//Get a reference to the player controller
		AAIZombieController* AIController = Cast<AAIZombieController>(GetController());
		if (AIController)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Zombie: I can see you !!!"));
			AIController->SetPlayerCaught(Pawn);
		}
	}
}

void AAIZombie::OnZombieOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!isDead) {
		if (OverlappedComponent->GetCollisionObjectType() == COLLISION_PLAYER) {
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("PLAYER"));
			GetMesh()->PlayAnimation(attackAnimation, false);
			AZombiETSCharacter* player = Cast<AZombiETSCharacter>(OtherActor);
			if (player != nullptr) {
				player->UpdateHealth(-100);
			}
		}
		else if (OverlappedComponent->GetCollisionObjectType() == COLLISION_SWORD) {
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("SWORD"));
			UpdateHealth(-500);
			if (ZombieHealth <= 0) {
				OnPlayerCaught(nullptr);
				GetMesh()->PlayAnimation(deathAnimation, false);
				SetLifeSpan(5);
				AlertGameModeOfDeath();
				//Destroy();
				isDead = true;
			}
			else {
				GetMesh()->PlayAnimation(hitAnimation, false);
			}
		}
	}
}

float AAIZombie::GetSpeedMultiplier()
{
	return speedMultiplier;
}

float AAIZombie::SetSpeedMultiplier(float speed)
{
	return speedMultiplier = speed;
}

/*HEALTH*/
float AAIZombie::GetInitialHealth()
{
	return InitialHealth;
}

float AAIZombie::GetCurrentHealth()
{
	return ZombieHealth;
}

void AAIZombie::UpdateHealth(float HealthChange)
{
	// Change Character'health
	ZombieHealth += HealthChange;

}

