#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Asteroid.generated.h"

class UStaticMeshComponent;

UCLASS()
class TP1_MOTEURDEJEU_API AAsteroid : public AActor
{
	GENERATED_BODY()

public:
	AAsteroid();

	virtual void Tick(float DeltaTime) override;

	// Retire de la vie à l'astéroïde (appelé par le projectile)
	UFUNCTION(BlueprintCallable, Category = "Astéroïde")
	void TakeHit(int32 Damage);

	// Force initiale, définie par le spawner
	UPROPERTY(BlueprintReadWrite, Category = "Astéroïde")
	FVector Velocity = FVector::ZeroVector;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Astéroïde")
	UStaticMeshComponent* Mesh;

	// Nombre de tirs nécessaires : aléatoire entre Min et Max
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astéroïde")
	int32 MinHealth = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astéroïde")
	int32 MaxHealth = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astéroïde")
	int32 ScoreValue = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astéroïde")
	float MinScale = 0.7f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astéroïde")
	float MaxScale = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astéroïde")
	float MaxSpinSpeed = 90.f;

	UPROPERTY(BlueprintReadOnly, Category = "Astéroïde")
	int32 Health = 1;

	// Effets à créer dans le Blueprint (Niagara, son...)
	UFUNCTION(BlueprintImplementableEvent, Category = "Astéroïde")
	void OnHitFX(FVector Location);

	UFUNCTION(BlueprintImplementableEvent, Category = "Astéroïde")
	void OnDestroyedFX(FVector Location);

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	void Explode(bool bGivePoints);

	FRotator SpinRate = FRotator::ZeroRotator;
};
