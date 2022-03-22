#include "xsAll.h"
#include "xs.h"
#include "mc.xs.h"
#include "mc.defines.h"

typedef struct ArchiveFileMappingStruct ArchiveFileMappingRecord, *ArchiveFileMapping;
struct ArchiveFileMappingStruct {
	xsSlot* archive;
	void* address;
	size_t size;
#if mxWindows
	
#else 
	int file;
#endif
};

static void ArchiveFileMappingMark(xsMachine* the, void* it, xsMarkRoot markRoot);

static xsBooleanValue remapped = 0;

static xsBooleanValue fxArchiveRead(void* src, size_t offset, void* buffer, size_t size)
{
	c_memcpy(buffer, ((txU1*)src) + offset, size);
	return 1;
}

static xsBooleanValue fxArchiveWrite(void* dst, size_t offset, void* buffer, size_t size)
{
	remapped = 1;
	c_memcpy(((txU1*)dst) + offset, buffer, size);
	return 1;
}

static xsHostHooks ArchiveFileMappingHooks = {
	ArchiveFileMappingDelete,
	ArchiveFileMappingMark,
	NULL
};

void ArchiveFileMappingCreate(xsMachine* the) 
{
	void* preparation = xsPreparation();
	ArchiveFileMappingRecord record;
	ArchiveFileMapping self = &record;
	c_memset(self, 0, sizeof(record));
#if mxWindows
	
#else 
	self->file = -1;
#endif
	xsTry {
		xsStringValue path = xsToString(xsArg(0));
	#if mxWindows
	
	#else 
		struct stat statbuf;
		self->file = open(path, O_RDWR);
		if (self->file < 0)
			xsUnknownError("%s", strerror(errno));
		fstat(self->file, &statbuf);
		self->size = statbuf.st_size;
		self->address = mmap(NULL, self->size, PROT_READ|PROT_WRITE, MAP_SHARED, self->file, 0);
		if (self->address == MAP_FAILED) {
			self->address = NULL;
			xsUnknownError("%s", strerror(errno));
		}
	#endif
		remapped = 0;
		fxMapArchive(the, preparation, self->address, 4 * 1024, fxArchiveRead, fxArchiveWrite);
		if (remapped)
			xsLog("# remap archive %s\n", path);
		xsResult = xsNewHostObject(NULL);
		xsSetHostBuffer(xsResult, self->address, self->size);
		self->archive = xsToReference(xsResult);
		xsSetHostChunk(xsThis, self, sizeof(record));
		xsSetHostHooks(xsThis, &ArchiveFileMappingHooks);
		xsResult = xsThis;
	}
	xsCatch {
		ArchiveFileMappingDelete(self);
		xsThrow(xsException);
	}
}

void ArchiveFileMappingDelete(void* it)
{
	if (it) {
		ArchiveFileMapping self = it;
#if mxWindows
	
#else 
		if (self->address) {
			munmap(self->address, self->size);
			self->address = NULL;
			self->size = 0;
		}
		if (self->file >= 0) {
			close(self->file);
			self->file = -1;
		}
#endif
	}
}

void ArchiveFileMappingMark(xsMachine* the, void* it, xsMarkRoot markRoot)
{
	ArchiveFileMapping self = it;
	if (self->archive) (*markRoot)(the, self->archive);
}

void ArchiveFileMapping_close(xsMachine *the)
{
	ArchiveFileMapping* self = xsGetHostHandle(xsThis);
	(*self)->archive = NULL;
	ArchiveFileMappingDelete(*self);
}

void ArchiveFileMapping_get_archive(xsMachine *the)
{
	ArchiveFileMapping* self = xsGetHostHandle(xsThis);
	xsResult = xsReference((*self)->archive);
}
