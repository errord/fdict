#ifndef _FDICT_DATAFILE_PARSE_H_
#define _FDICT_DATAFILE_PARSE_H_

/*
 *
 * FLDF (Field List Data File)
 * field1\tfield2\tfield3
 *
 */

bool fldata_parse(struct fdict_s *fdict, struct record_s *record, char *data);


#endif /* _FDICT_DATAFILE_PARSE_H_ */
