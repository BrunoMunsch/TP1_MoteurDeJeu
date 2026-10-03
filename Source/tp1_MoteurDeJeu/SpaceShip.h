#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "SpaceShip.generated.h"

class UStaticMeshComponent;
class UInputAction;
class UInputMappingContext;
class AProjectile;

UCLASS()
class TP1_MOTEURDEJEU_API ASpaceShip : public APawn
{
	GENERATED_BODY()

public:
	ASpaceShip();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);
	void StopMove(const FInputActionValue& Value);
	void Fire(const FInputActionValue& Value);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vaisseau")
	UStaticMeshComponent* Mesh;

	// Entrées (à assigner dans le Blueprint)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Entrées")
	UInputMappingContext* MappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Entrées")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Entrées")
	UInputAction* FireAction;

	// Réglages (ajustables dans le Blueprint)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vaisseau")
	float MoveSpeed = 1220.f;

	// Demi-dimensions de la zone de jeu (X = haut/bas, Y = gauche/droite)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vaisseau")
	float BoundsX = 800.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vaisseau")
	float BoundsY = 1400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tir")
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tir")
	float FireCooldown = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tir")
	float MuzzleOffset = 120.f;

	// Pour jouer un son ou un effet Niagara dans le Blueprint
	UFUNCTION(BlueprintImplementableEvent, Category = "Tir")
	void OnFireFX();

private:
	FVector2D MoveInput = FVector2D::ZeroVector;
	float LastFireTime = -1000.f;
};
