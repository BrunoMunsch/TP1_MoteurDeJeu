#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AsteroidSpawner.generated.h"

class AAsteroid;

UCLASS()
class TP1_MOTEURDEJEU_API AAsteroidSpawner : public AActor
{
	GENERATED_BODY()

public:
	AAsteroidSpawner();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	TSubclassOf<AAsteroid> AsteroidClass;

	// Même zone que celle du vaisseau (demi-dimensions)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	float HalfExtentX = 800.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	float HalfExtentY = 1400.f;

	// Distance hors de la zone visible où les astéroïdes apparaissent
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	float Margin = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	float MinDelay = 0.6f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	float MaxDelay = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	float MinSpeed = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	float MaxSpeed = 700.f;

	// Probabilité (0 à 1) qu'un astéroïde vise le joueur
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float AimAtPlayerChance = 0.5f;

private:
	void SpawnAsteroid();
	void ScheduleNext();

	FTimerHandle SpawnTimer;
};
