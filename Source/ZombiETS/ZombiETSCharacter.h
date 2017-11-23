// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "ZombiETSCharacter.generated.h"

UCLASS(config=Game)
class AZombiETSCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Collection sphere */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CollectionSphere;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	AZombiETSCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	// Getter Initial Health
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetInitialHealth();

	// Getter Current Health
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetCurrentHealth();

	/**
	* Function to update character's health (security, disable modification of variable withtout accessing code)
	* @param HealthChange : The variable can be + or -
	*/
	UFUNCTION(BlueprintCallable, Category = "Health")
	void UpdateHealth(float HealthChange);

	UFUNCTION(BlueprintPure, Category = "Speed")
	static float GetSpeedMultiplier();
	UFUNCTION(BlueprintPure, Category = "Speed")
	static float SetSpeedMultiplier(float speed);

	float baseSpeed;
	static float speedMultiplier;


protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	// Called when we press the collect key (c)
	UFUNCTION(BlueprintCallable, Category = "Pickups")
	void CollectPickups();

	// The starting health of main character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float InitialHealth;

	UFUNCTION(BlueprintImplementableEvent, category = "Health")
	void HealthChangeEffect();

private:
	// Current health of the main character
	UPROPERTY(VisibleAnywhere, Category = "Health")
	float CharacterHealth;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	// Returns CollectionSphere subObject
	FORCEINLINE class USphereComponent* GetCollectionSphere() const { return CollectionSphere; }
};

