getwd()

list.of.packages <- c("caret", "ROCR","e1071","gmodels","glmnet","class","arm","ggplot2","PerformanceAnalytics","ggcorrplot","GGally","pROC","survival","Hmisc","leaps")
new.packages <- list.of.packages[!(list.of.packages %in% installed.packages()[,"Package"])]
if(length(new.packages)) install.packages(new.packages)

library(caret) #permite a utiliza��o de modelos de machine learning
library(ROCR) #permite criar a curva para avaliar a performance do modelo
library(e1071) #cont�m fun��es e algoritmos utilizados no script
library(gmodels)
library(class)
library(arm)
library(glmnet)
library(ggplot2)
library(PerformanceAnalytics)
library(ggcorrplot)
library(GGally)
library(pROC)
library(survival)
library(Hmisc)
library(dplyr)
library(leaps)


# 569 observacoes - com 32 variáveis. B:Benigno - M:Maligno
dados <- read.csv("dataset.csv", stringsAsFactors = FALSE)  #Reader

##### PRE-PROCESSAMENTO #####
dados$id = NULL # ID Column Removed

# Ajustando o label da variavel alvo

dados$diagnosis<-ifelse(dados$diagnosis == "B", 0, 1)

#Organização dos dados por Categoria (Mean, SE, WORST)
data_mean = cbind(diagnosis=dados[,c(1)], dados[,c(2:11)])
data_se = cbind(diagnosis=dados[,c(1)], dados[,c(12:21)])
data_worst = cbind(diagnosis=dados[,c(1)], dados[,c(22:31)])


#Histograma
par(mar = rep(2, 4))
hist.data.frame(dados, n.unique=1, mtitl = "Histograma de Cancro de Mama")

#Histograma por Categoria
hist.data.frame(data_mean[,-1], n.unique=1, mtitl = "Histograma de Cancro de Mama - Média")  
hist.data.frame(data_se[,-1], n.unique=1, mtitl = "Histograma de Cancro de Mama - Erro Padrão")  
hist.data.frame(data_worst[,-1], n.unique=1, mtitl = "Histograma de Cancro de Mama - Pior Caso")  


#Boxplot por categoria
par(mfrow=c(1,1)) #reset mfrow parameters
par(cex.axis=0.8) # is for x-axis
boxplot(data_mean[,-1], las=2, col="green", main="Box-Plot Cancer de Mama - Mean", ylim = c(0,150))
boxplot(data_se[,-1], las=2, col="green", main="Box-Plot Cancer de Mama - SE", ylim = c(0,150))
boxplot(data_worst[,-1], las=2, col="green", main="Box-Plot Cancer de Mama - Worst", ylim = c(0,150))


#Chart correlation 
#Mean
chart.Correlation(data_mean,histogram=TRUE,pch=19)
#SE
chart.Correlation(data_se,histogram=TRUE,pch=19)
#Worst
chart.Correlation(data_worst,histogram=TRUE,pch=19)

#Matrix Correlation
ggcorr(dados, nbreaks=8, palette='PRGn', label=TRUE, label_size=2, size = 1.8, label_color='black') + ggtitle("Breast Cancer Correlation Matrix") + theme(plot.title = element_text(hjust = 0.5, color = "grey15"))


#Distribuição Variável Resposta
dados$diagnosis = sapply(dados$diagnosis, function(x){ifelse(x==1, 'Maligno', 'Benigno')})
barplot(table(dados$diagnosis), col="blue", ylab="count", xlab="Diagnosis", main="Response distribution")


#Mudar a classificação da variável diagnosis para tipo "Factor"
table(dados$diagnosis)
dados$diagnosis<-ifelse(dados$diagnosis == "Benigno", 0, 1)
dados$diagnosis <- factor(dados$diagnosis,levels = c(0, 1))
str(dados$diagnosis)


# Ao analisar a base de dados observou-se qua há um problema de escala entre os dados
# que se encontram fora de escala e preisam ser padronizados
summary(dados[2:31])

#Escalonamento os valores dos preditores quantitativos
dados_z <- as.data.frame(scale(dados[-1]))
summary(dados_z)
cor(dados_z)

#Unindo os dados normalizados com a coluna de diagnosis através da função cbind
dados_bind <- cbind(dados$diagnosis, dados_z)
names(dados_bind)[1] <- "diagnosis"
str(dados_bind)
View(dados_bind)




############################## REGRESSÃO LOGÍSTICA ##########################################
#base de dados com dados normalizados
glm.fit=glm(diagnosis ~., data=dados_bind, family=binomial, control = list(maxit = 33))
summary(glm.fit)

###### Separando dataset de treino e teste #####
#Modelo 1
set.seed(123)
indx<-createDataPartition(dados_bind$diagnosis, p=0.7, list=FALSE)
train_dados<-dados_bind[indx,]
train_class<-dados_bind$diagnosis[indx] 
test_dados<-dados_bind[-indx,-1]
test_class<-dados_bind$diagnosis[-indx]

model_log1 <-glm(diagnosis ~ . ,family=binomial, train_dados)
summary(model_log1)

conv_log1 <- ifelse(predict(object=model_log1, newdata=test_dados, type="response") > 0.5,1,0)
confusionMatrix(table(conv_log1, test_class), positive="1")
conf_log1 <- confusionMatrix(table(conv_log1, test_class), positive="1")


#Correlação de dados - preditores mais significantes
z = cor(dados_z)
z = round(z,4)
z[abs(z)<0.9]=NA #remove preditores com baixo relacionamneto
z[lower.tri(z,diag=TRUE)]=NA  #descarta informações duplicadas e sem influência sobre a variável alvo
z=as.data.frame(as.table(z))  #Transforme em uma tabela de 3 colunas
z=na.omit(z)  #livra-se do lixo que sinalizado acima
z=z[order(-abs(z$Freq)),]    #Classifica pela correlação mais alta
z


#Modelo 2 - Baseado nas correlações de dados ('z')
subdados<-subset(dados_bind, select=-c(area_mean,radius_worst,area_worst,radius_mean,radius_se,area_se,
                                       perimeter_mean,perimeter_worst,perimeter_se,points_mean, points_worst,
                                       texture_mean,texture_worst,concavity_mean))
set.seed(123)
train2_dados<-subdados[indx,]
train2_class<-subdados$diagnosis[indx] 
test2_dados<-subdados[-indx,-1]
test2_class<-subdados$diagnosis[-indx]


model_log2 <-glm(diagnosis ~ . ,family=binomial, train2_dados)
summary(model_log2)

conv_log2 <- ifelse(predict(object=model_log2, newdata=test2_dados, type="response") > 0.5,1,0)
confusionMatrix(table(conv_log2, test2_class), positive="1")
conf_log2 <- confusionMatrix(table(conv_log2, test2_class), positive="1")
 


#Modelo 3 - Modelo 2 atualizado
subdados2 <- subset(subdados, select=c(diagnosis,compactness_se,dimension_mean,texture_se,concavity_se,points_se,
                                          compactness_worst,concavity_worst,symmetry_worst,dimension_worst))

set.seed(123)
train3_dados<-subdados2[indx,]
train3_class<-subdados2$diagnosis[indx] 
test3_dados<-subdados2[-indx,-1]
test3_class<-subdados2$diagnosis[-indx]

model_log3 <-glm(diagnosis ~ . ,family=binomial, train3_dados)
summary(model_log3)

conv_log3 <- ifelse(predict(object=model_log3, newdata=test3_dados, type="response") > 0.5,1,0)
confusionMatrix(table(conv_log3, test3_class), positive="1")
conf_log3 <- confusionMatrix(table(conv_log3, test3_class), positive="1")



###########Filtrar Subsets e Testar novos modelos #######################

#A função #regsubsets () executa a melhor seleção de subconjunto identificando o melhor modelo 
#que contém um determinado número de preditores, onde o melhor é quantificado usando RSS
regfit.full=regsubsets(diagnosis~.,dados_bind)
summary(regfit.full)
# best one-variable model contains only CRBI
# best two-variable model contains Hits and CRBI
dim(dados_bind)
regfit.full=regsubsets(diagnosis~.,data=dados_bind,nvmax=30) #let's fit up to a 30-variable model
reg.summary=summary(regfit.full)
names(reg.summary)
reg.summary$rsq

par(mfrow=c(1,1))
par(mfrow=c(2,2))
plot(reg.summary$rss,xlab="Number of Variables",ylab="RSS",type="l")
plot(reg.summary$adjr2,xlab="Number of Variables",ylab="Adjusted RSq",type="l")
which.max(reg.summary$adjr2)
points(14,reg.summary$adjr2[13], col="red",cex=2,pch=20)
plot(reg.summary$cp,xlab="Number of Variables",ylab="Cp",type='l')
which.min(reg.summary$cp)
points(14,reg.summary$cp[12],col="red",cex=2,pch=20)
which.min(reg.summary$bic)
plot(reg.summary$bic,xlab="Number of Variables",ylab="BIC",type='l')
points(11,reg.summary$bic[11],col="red",cex=2,pch=20)

?regsubsets
# a linha superior de cada gráfico contém um quadrado preto para cada variável 
# selecionado de acordo com o modelo ideal associado a essa estatística
plot(regfit.full,scale="r2")
plot(regfit.full,scale="adjr2")
plot(regfit.full,scale="Cp")
plot(regfit.full,scale="bic")
coef(regfit.full,11)


#######CP - 14 preditores#####
subdataset<-subset(dados_bind, select=c(diagnosis,radius_mean,compactness_mean,concavity_mean,points_mean,radius_se,smoothness_se,
                                        concavity_se,points_se,radius_worst,texture_worst,area_worst,concavity_worst,symmetry_worst,
                                          dimension_worst))
set.seed(123)
train4_dados<-subdataset[indx,]
train4_class<-subdataset$diagnosis[indx] 
test4_dados<-subdataset[-indx,-1]
test4_class<-subdataset$diagnosis[-indx]

model_log4 <-glm(diagnosis ~ . ,family=binomial, train4_dados)
summary(model_log4)

conv_log4 <- ifelse(predict(object=model_log4, newdata=test4_dados, type="response") > 0.5,1,0)
confusionMatrix(table(conv_log4, test4_class), positive="1")
conf_log4 <- confusionMatrix(table(conv_log4, test4_class), positive="1")


########adj2 - 14 preditores####
subdataset<-subset(dados_bind, select=c(diagnosis,radius_mean,compactness_mean,concavity_mean,points_mean,
                                         radius_se,smoothness_se,concavity_se,points_se,radius_worst,
                                         texture_worst,area_worst,concavity_worst,symmetry_worst,dimension_worst))
set.seed(123)
train5_dados<-subdataset[indx,]
train5_class<-subdataset$diagnosis[indx] 
test5_dados<-subdataset[-indx,-1]
test5_class<-subdataset$diagnosis[-indx]

model_log5 <-glm(diagnosis ~ . ,family=binomial, train5_dados)
summary(model_log5)

conv_log5 <- ifelse(predict(object=model_log5, newdata=test5_dados, type="response") > 0.5,1,0)
confusionMatrix(table(conv_log5, test5_class), positive="1")
conf_log5 <- confusionMatrix(table(conv_log5, test5_class), positive="1")


#######BIC - 11 preditores#####
subdataset<-subset(dados_bind, select=c(diagnosis,compactness_mean,concavity_mean,radius_se,smoothness_se,concavity_se,
                                          radius_worst,texture_worst,area_worst,points_worst,symmetry_worst,
                                          dimension_worst))
set.seed(123)
train6_dados<-subdataset[indx,]
train6_class<-subdataset$diagnosis[indx] 
test6_dados<-subdataset[-indx,-1]
test6_class<-subdataset$diagnosis[-indx]

model_log6 <-glm(diagnosis ~ . ,family=binomial, train6_dados)
summary(model_log6)

conv_log6 <- ifelse(predict(object=model_log6, newdata=test6_dados, type="response") > 0.5,1,0)
confusionMatrix(table(conv_log6, test6_class), positive="1")
conf_log6 <- confusionMatrix(table(conv_log6, test6_class), positive="1")


# Forward and Backward Stepwise Selection
regfit.fwd=regsubsets(diagnosis~.,data=dados_bind,nvmax=30,method="forward")
summary(regfit.fwd)
regfit.bwd=regsubsets(diagnosis~.,data=dados_bind,nvmax=30,method="backward")
summary(regfit.bwd)
coef(regfit.full,11)
coef(regfit.fwd,11)
coef(regfit.bwd,11)


###Foward Stpewise - 11 preditores
subdataset<-subset(dados_bind, select=c(diagnosis,compactness_mean,points_mean,radius_se,smoothness_se,compactness_se,
                                          radius_worst,texture_worst,area_worst,points_worst,symmetry_worst,
                                          dimension_worst))
set.seed(123)
train7_dados<-subdataset[indx,]
train7_class<-subdataset$diagnosis[indx] 
test7_dados<-subdataset[-indx,-1]
test7_class<-subdataset$diagnosis[-indx]

model_log7 <-glm(diagnosis ~ . ,family=binomial, train7_dados)
summary(model_log7)

conv_log7 <- ifelse(predict(object=model_log7, newdata=test7_dados, type="response") > 0.5,1,0)
confusionMatrix(table(conv_log7, test7_class), positive="1")
conf_log7 <- confusionMatrix(table(conv_log7, test7_class), positive="1")



###Backward Stpewise - 11 preditores
subdataset<-subset(dados_bind, select=c(diagnosis,compactness_mean,points_mean,radius_se,smoothness_se,concavity_se,
                                          radius_worst,texture_worst,area_worst,concavity_worst,symmetry_worst,
                                          dimension_worst))
set.seed(123)
train8_dados<-subdataset[indx,]
train8_class<-subdataset$diagnosis[indx] 
test8_dados<-subdataset[-indx,-1]
test8_class<-subdataset$diagnosis[-indx]

model_log8 <-glm(diagnosis ~ . ,family=binomial, train8_dados)
summary(model_log8)

conv_log8 <- ifelse(predict(object=model_log8, newdata=test8_dados, type="response") > 0.5,1,0)
confusionMatrix(table(conv_log8, test8_class), positive="1")
conf_log8 <- confusionMatrix(table(conv_log8, test8_class), positive="1")


#ExtractAIC
extractAIC(model_log1); extractAIC(model_log2); extractAIC(model_log3); extractAIC(model_log4);
extractAIC(model_log5); extractAIC(model_log6); extractAIC(model_log7); extractAIC(model_log8)








#################################### MODELO KNN ################################################

# Criando dados de treino e dados de teste
View(dados_bind)
set.seed(123)

indx2<-createDataPartition(dados_bind$diagnosis, p=0.7, list=FALSE)
dadosk_treino <- dados_bind[indx2, -1]
dadosk_teste <- dados_bind[-indx2, -1]
dim(dadosk_treino)
dim(dadosk_teste)
class(dadosk_treino)

# Criando os labels para os dados de treino e de teste
dados_treino_labels <- dados_bind[indx2, 1]
dados_teste_labels <- dados_bind[-indx2, 1]
length(dados_treino_labels)
length(dados_teste_labels)

#Encontrando o valor final para o "k"
dadosknn_treino <- dados_bind[indx2,]
ctrl <- trainControl(method="repeatedcv",number=10, repeats = 10)
knnFit <- train(diagnosis ~ ., data = dadosknn_treino, method = "knn", trControl = ctrl, preProcess = c("center","scale"),tuneLength = 10)
knnFit
?trainControl
# Criação do modelo
modelo_knn_v1 <- knn(train = dadosk_treino, 
                     test = dadosk_teste,
                     cl = dados_treino_labels, 
                     k = 7)

summary(modelo_knn_v1)
confusionMatrix(table(modelo_knn_v1, dados_teste_labels), positive="1")
CrossTable(x = dados_teste_labels, y = modelo_knn_v1, chisq = TRUE)



########## MÉTRICA - PERFORMANCE DOS MODELOS DE TESTE #####################
#Teste Acurácia
acc1 <- conf_log1$overall["Accuracy"]
acc2 <- conf_log2$overall["Accuracy"]
acc3 <- conf_log3$overall["Accuracy"]
acc4 <- conf_log4$overall["Accuracy"]
acc5 <- conf_log5$overall["Accuracy"]
acc6 <- conf_log6$overall["Accuracy"]
acc7 <- conf_log7$overall["Accuracy"]
acc8 <- conf_log8$overall["Accuracy"]
  
#Teste area sob a curva (Area Under Curve)
auc1 <- roc(test_class ~ conv_log1, test_dados)$auc
auc2 <- roc(test2_class ~ conv_log2, test2_dados)$auc
auc3 <- roc(test3_class ~ conv_log3, test3_dados)$auc
auc4 <- roc(test4_class ~ conv_log4, test4_dados)$auc
auc5 <- roc(test5_class ~ conv_log5, test5_dados)$auc
auc6 <- roc(test6_class ~ conv_log6, test6_dados)$auc
auc7 <- roc(test7_class ~ conv_log7, test7_dados)$auc
auc8 <- roc(test8_class ~ conv_log8, test8_dados)$auc

df<-data.frame(accuracy=c(acc1,acc2,acc3,acc4,acc5,acc6,acc7,acc8),auc=c(auc1,auc2,auc3,auc4,auc5,auc6,auc7,auc8))
row.names(df)<-c("Logistic - M1","Logistic - M2","Logistic - M3","Logistic - M4","Logistic - M5","Logistic - M6","Logistic - M7","Logistic - M8")
View(df)

extractAIC(model_log1); extractAIC(model_log2); extractAIC(model_log3); extractAIC(model_log4);
extractAIC(model_log5); extractAIC(model_log6); extractAIC(model_log7); extractAIC(model_log8)

#prev_log2 <- prediction(conv_log2, test2_class)
previsoes <- predict(model_log4, test4_dados, type = "response")
previsoes_finais <- prediction(previsoes, test4_class)

# Função para Plot ROC 
plot.roc.curve <- function(predictions, title.text){
  perf <- performance(predictions, "tpr", "fpr")
  plot(perf,col = "black",lty = 1, lwd = 2,
       main = title.text, cex.main = 0.6, cex.lab = 0.8,xaxs = "i", yaxs = "i")
  abline(0,1, col = "red")
  auc <- performance(predictions,"auc")
  auc <- unlist(slot(auc, "y.values"))
  auc <- round(auc,2)
  legend(0.4,0.4,legend = c(paste0("AUC: ",auc)), cex = 0.6, bty = "n", box.col = "white")
}

# Plot
par(mfrow = c(1, 2))
plot.roc.curve(previsoes_finais, title.text = "Curva ROC")


