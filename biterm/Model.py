import biterm.BTM as __btm__
class bitermModel(object):
	def __init__(self,k,w,a,b,n_iter,save_step):
		"""
		k:topics,
		w:vocaburary size,
		a:alpha,b:belta,
		n_iter:gibss sample iter, 
		save_step:savestep
		"""
		self._handle=__btm__.initmodel(k,w,a,b,n_iter,save_step)
	
	def __del__(self):
		__btm__.delete(self._handle)
	
	def train(self,filename):
		"""
		filename:one record per line,wordid delimited by space
		"""
		__btm__.train(self._handle,filename)
	
	def predict(self,record):
		record=list(record)
		relist=__btm__.predict(self._handle,record)
		return relist
	def savemodel(self,filedir):
		"""
		filedir ,the dir ro save model
		"""
		__btm__.savemodel(self._handle,filedir)

	def loadmodel(self,filedir):
		__btm__.loadmodel(self._handle,filedir)
