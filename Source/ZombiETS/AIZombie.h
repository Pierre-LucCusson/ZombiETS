// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "AIZombie.generated.h"

UCLASS()
class ZOMBIETS_API AAIZombie : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAIZombie(const FObjectInitializer& objectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// The starting health of main character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float InitialHealth;

	// Alert the game mode of the zombie's death
	void AlertGameModeOfDeath();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = AI)
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(VisibleAnywhere, Category = AI)
	class UPawnSensingComponent* PawnSensingComp;

	UFUNCTION()
	void OnZombieOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintPure, Category = "Speed")
	static float GetSpeedMultiplier();
	UFUNCTION(BlueprintPure, Category = "Speed")
	static float SetSpeedMultiplier(float speed);

	// Getter Initial Health
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetInitialHealth();

	// Getter Current Health
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetCurrentHealth();

	UFUNCTION(BlueprintCallable, Category = "Health")
	void UpdateHealth(float HealthChange);

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimSequence *deathAnimation;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimSequence *hitAnimation;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimSequence *attackAnimation;

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void PlayAnimation(UAnimationAsset* animation, float time);

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void SetAnimationBlueprintMode();

private:

	UFUNCTION()
	void OnPlayerCaught(APawn* Pawn);

	USphereComponent* overlapSwordColision;
	USphereComponent* overlapPlayerColision;

	bool isDead;
	bool isHit;

	float baseSpeed;
	static float speedMultiplier;

	// Current health of the main character
	UPROPERTY(VisibleAnywhere, Category = "Health")
	float ZombieHealth;
};
