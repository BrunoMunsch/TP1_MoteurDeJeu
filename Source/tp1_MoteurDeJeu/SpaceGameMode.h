#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpaceGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStatsChanged);

UCLASS()
class TP1_MOTEURDEJEU_API ASpaceGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Jeu")
	void AddScore(int32 Points);

	UFUNCTION(BlueprintCallable, Category = "Jeu")
	void LoseLife();

	UPROPERTY(BlueprintReadOnly, Category = "Jeu")
	int32 Score = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Jeu")
	int32 Lives = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jeu")
	int32 StartLives = 3;

	// À lier dans le widget pour rafraîchir le score et les vies
	UPROPERTY(BlueprintAssignable, Category = "Jeu")
	FOnStatsChanged OnStatsChanged;

	// À implémenter dans le Blueprint (écran de fin, retour au menu, etc.)
	UFUNCTION(BlueprintImplementableEvent, Category = "Jeu")
	void OnGameOver();

protected:
	virtual void BeginPlay() override;

private:
	bool bGameOver = false;
};
